/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    AssetManager.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 10:46:00 - nwilliam
**  \brief   Implementation of asset loader, performed with the tasks added to
             global task manager.
*******************************************************************************/

#include "AssetManager.h"
#include "SolarFlare/Globals.h"           // console, taskManager
#include "Utilities/DevConsole.h"
#include "Threading/TaskManager.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace
{
  void FileLoadWrapper( const SF::Engine::FileLoadParams& params )
  {
    params.asset->Load();
  }
}


namespace SF {
namespace Engine
{
  //////////////////////////////////////////////////////////////////////////////
  // FileLoadParams constructor
  //////////////////////////////////////////////////////////////////////////////
  FileLoadParams::FileLoadParams( Assets::BaseAsset* _asset ) : asset( _asset )
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Constructor, destructor
  //////////////////////////////////////////////////////////////////////////////
  AssetManager::AssetManager()
  {
  }

  AssetManager::~AssetManager()
  {
    for( AssetTable::iterator it = m_assets.begin(); it != m_assets.end(); ++it )
    {
      delete *it;
    }

    for( AssetTable::iterator it = m_fileLoaders.begin(); it != m_fileLoaders.end(); ++it )
    {
      delete *it;
    }

    m_assets.clear();
    m_fileLoaders.clear();
    m_fileSubfolders.clear();
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  void AssetManager::LoadAssets( const std::string& filename )
  {
    try
    {
      ticpp::Document assetDoc( filename );
      assetDoc.LoadFile();

      // Make sure top level is AssetList (throws if it isn't)
      ticpp::Element* assetList = assetDoc.FirstChildElement( "AssetList" );

      ticpp::Iterator< ticpp::Element > normal( assetList->FirstChildElement( "asset" ), "asset" );
      ticpp::Iterator< ticpp::Element > critical( assetList->FirstChildElement( "asset_c" ), "asset_c" );

      for( int i = 0; i < 2; ++i )
      {
        ticpp::Iterator< ticpp::Element > asset = (i == 0) ? critical : normal;

        while( asset != asset.end() )
        {
          std::string attr;
          std::string mappedName;

          asset->GetAttribute( "file", &attr, false );
          asset->GetAttribute( "name", &mappedName, false );

          if( attr.size() != 0 )
          {
            LoadFile( attr, mappedName, i == 0 );
          }
          else
          {
            Globals::console->Error() << "Invalid asset tag, line: " 
                                      << asset->Row() << std::endl;
          }

          ++asset;
        }
      }
    }
    catch( const ticpp::Exception& e )
    {
      Globals::console->Error() << "LoadAssets Exception: "
                                << e.what() << std::endl;
    }

    Globals::taskManager->WaitForCriticalTasks();
  }

  Assets::BaseAsset* AssetManager::GetAsset( const std::string& mappedName )
  {
    AssetTable::iterator it = m_assets.find( mappedName );

    if( it == m_assets.end() )
      return 0;

    return *it;
  }

  const std::string& AssetManager::GetDirectory()
  {
    return m_assetDirectory;
  }

  void AssetManager::SetDirectory( const std::string& relativePath )
  {
    m_assetDirectory = relativePath;
  }

  std::string AssetManager::GetAssetTypeDirectory( const std::string& ext )
  {
    StringTable::iterator it = m_fileSubfolders.find( ext );

    std::string dir;

    if( it != m_fileSubfolders.end() )
    {
      dir = m_assetDirectory + *it;
    }

    return dir;
  }

  void AssetManager::ReloadType( const Assets::Types type )
  {
    for( AssetTable::iterator it = m_assets.begin(); it != m_assets.end(); ++it )
    {
      if( (*it)->type == type )
      {
        ReloadFile( (*it)->name );
      }
    }
  }

  Assets::BaseAsset* AssetManager::ReloadFile( const std::string& mappedName )
  {
    AssetTable::iterator it = m_assets.find( mappedName );

    if( it != m_assets.end() )
    {
      if( (*it)->Load() )
      {
        (*it)->Swap();

        std::string msg( "Reloading of asset successful: " );
        msg += mappedName;
        Globals::console->Debug( msg );

        return *it;;
      }
    }

    std::string err( "Reloading of asset failed: " );
    err += mappedName;
    Globals::console->Error( err );

    return 0;
  }

  Assets::BaseAsset* AssetManager::LoadFile( const std::string& filename, const std::string& mappedName, bool critical )
  {
    std::string ext = FindExtension( filename );
    AssetTable::iterator loader = m_fileLoaders.find( ext );

    Assets::BaseAsset* asset = 0;

    if( loader == m_fileLoaders.end() )
    {
      std::string error( "No loader for file ext: " );
      error += ext;
      Globals::console->Error( error );
      return asset;
    }

    if( m_assets.find( mappedName ) != m_assets.end() )
    {
      std::string error( "Asset already exists for name: " );
      error += mappedName;
      Globals::console->Error( error );
      return asset;
    }

    asset = (*loader)->New();

    std::string& subfolder = *(m_fileSubfolders.find( ext ));

    asset->name = mappedName;
    asset->filePath = m_assetDirectory + subfolder + filename;
    asset->type = (*loader)->type;

    FileLoadParams params( asset );
    Globals::taskManager->AddTask( FileLoadWrapper, params, SwapCallback::Method<Assets::BaseAsset, &Assets::BaseAsset::Swap>(params.asset), critical );

    m_assets.insert( mappedName, asset );

    return asset;
  }

  void AssetManager::AddFileType( const std::string& ext, const std::string& folder, const Assets::Types type, const Assets::BaseAsset& loader )
  {
    AssetTable::iterator it = m_fileLoaders.find( ext );

    if( it != m_fileLoaders.end() )
    {
      std::string warn( "Overriding asset loader for ext: " );
      Globals::console->Warning( warn + ext );

      m_fileLoaders.erase( it );
      m_fileSubfolders.erase( ext );
    }

    Assets::BaseAsset* bap = loader.New();
    bap->type = type;
    m_fileLoaders.insert( ext, bap );
    m_fileSubfolders.insert( ext, folder );
  }

  //////////////////////////////////////////////////////////////////////////////
  // Private Methods
  //////////////////////////////////////////////////////////////////////////////
  std::string AssetManager::FindExtension( const std::string& filename )
  {
    std::string ext;
    std::string::size_type pos = filename.rfind( '.' );

    if( pos != std::string::npos )
    {
      ext = filename.substr( pos + 1 );
    }

    return ext;
  }

} // Engine namespace
} // SF namespace
