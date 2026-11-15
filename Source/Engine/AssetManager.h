/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    AssetManager.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/04/2009 @ 18:48:22 - nwilliam
**  \brief   Manager for loading and creating assests.
*******************************************************************************/

#ifndef SF_ASSET_MANAGER_H
#define SF_ASSET_MANAGER_H

#include "Assets.h"
#include "Utilities/Delegates.h"
#include "Utilities/HashTable.h"
#include "Threading/TaskManager.h"

// Foward declaration
namespace ticpp { class Element; }

namespace SF {
namespace Engine
{
  typedef Utilities::Delegate0<void> SwapCallback;

  struct FileLoadParams
  {
    FileLoadParams( Assets::BaseAsset* _asset );
    Assets::BaseAsset* asset;
  };


  class AssetManager
  {
    public:
      AssetManager();
      ~AssetManager();

      void LoadAssets( const std::string& filename );
      Assets::BaseAsset* GetAsset( const std::string & mappedName );

      const std::string& GetDirectory();
      void SetDirectory( const std::string& relativePath );
      std::string GetAssetTypeDirectory( const std::string& ext );

      void ReloadType( Assets::Types type );
      Assets::BaseAsset* ReloadFile( const std::string& mappedName );
      Assets::BaseAsset* LoadFile( const std::string& filename, const std::string& mappedName, bool critical );

      void AddFileType( const std::string& ext, const std::string& folder, const Assets::Types type, const Assets::BaseAsset& loader );

    private:
      typedef Utilities::HashTable< std::string > StringTable;
      typedef Utilities::HashTable< Assets::BaseAsset* > AssetTable;

      AssetManager( const AssetManager& );            // No copy
      AssetManager& operator=( const AssetManager& ); // No assignment

      std::string FindExtension( const std::string& filename );

      AssetTable m_assets;
      AssetTable m_fileLoaders;
      std::string m_assetDirectory;
      StringTable m_fileSubfolders;
  };

} // Engine namespace
} // SF namespace

#endif // SF_ASSET_MANAGER_H
