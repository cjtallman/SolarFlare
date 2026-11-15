/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseModel.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/11/2009 @ 22:15:39 -nwilliam
**  \brief
*******************************************************************************/

#include "External/assimp/include/assimp.hpp"
#include "External/assimp/include/aiScene.h"
#include "External/assimp/include/aiPostProcess.h"
#include "External/assimp/include/DefaultLogger.h"
#include "External/assimp/include/Logstream.h"

#include "OpenGLInclude.h"
#include "Vertex.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"
#include "Utilities/Macros.h"

#include "BaseModel.h"
#include "Engine/Assets.h"

#include "Utilities/LeakCheck.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace SF
{
  namespace Graphics
  {
    namespace Model
    {
      class ErrorLogger : public Assimp::LogStream
      {
      public:
        ErrorLogger(){}
        ~ErrorLogger(){}
        void write(const std::string & message){Globals::console->Error(message);}
      };

      class WarningLogger : public Assimp::LogStream
      {
      public:
        WarningLogger(){}
        ~WarningLogger(){}
        void write(const std::string & message){Globals::console->Warning(message);}
      };

      class DebugLogger : public Assimp::LogStream
      {
      public:
        DebugLogger(){}
        ~DebugLogger(){}
        void write(const std::string & message){Globals::console->Debug(message);}
      };

      class InfoLogger : public Assimp::LogStream
      {
      public:
        InfoLogger(){}
        ~InfoLogger(){}
        void write(const std::string & message){Globals::console->Message(message);}
      };


      bool ModelError( const std::string & error )
      {
        Globals::console->Error( "BaseModel.cpp --- " + error );
        return false;
      }

      BaseModel::BaseModel()
        : mesh_count(0)
      {
        //Use default mesh object. ( simple cube )
        meshes.push_back( Mesh() );
        mesh_count++;
      }

      BaseModel::~BaseModel()
      {
        meshes.clear();
      }

      int BaseModel::VertexCount( int i )
      {
        if( i < -1 || i > static_cast<int>(mesh_count) )
        {
          ModelError( "Invalid Index for BaseModel::VertexCount" );
          return 0;
        }
        else if( i == -1 )
        {
          int count = 0;
          for( unsigned int mesh = 0; mesh < mesh_count; ++mesh )
            count += meshes[mesh].VertexCount();

          return count;
        }
        else
          return meshes[i].VertexCount();
      }

      const VertexBuffer3<Vertex::ModelVertex> * BaseModel::VertexBuffer( int i ) const
      {
        if( i < 0 || i > static_cast<int>(mesh_count) )
        {
          ModelError( "Invalid Index for BaseModel::VertexCount" );
          return 0;
        }
        else
          return &(meshes[i].VertexBuffer());
      }



      void BaseModel::Load( int i )
      {
        if( i == -1 )
          for( unsigned int mesh = 0; mesh < mesh_count; ++mesh )
            meshes[mesh].Load();

        else
          meshes[i].Load();
      }

      void BaseModel::Unload( int i )
      {
        if( i == -1 )
        {
          for( unsigned int mesh = 0; mesh < mesh_count; ++mesh )
            meshes[mesh].Unload();
        }
        else
          meshes[i].Unload();
      }


      bool BaseModel::LoadFromFile(const std::string &filename)
      {
        Assimp::Importer importer;

        /*Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);

        ErrorLogger error_logger;
        WarningLogger warning_logger;
        DebugLogger debug_logger;
        InfoLogger message_logger;

        Assimp::DefaultLogger::get()->attachStream( &error_logger, Assimp::Logger::ERR );
        Assimp::DefaultLogger::get()->attachStream( &warning_logger, Assimp::Logger::WARN );
        Assimp::DefaultLogger::get()->attachStream( &debug_logger, Assimp::Logger::DEBUGGING );
        Assimp::DefaultLogger::get()->attachStream( &message_logger, Assimp::Logger::INFO );
        Assimp::DefaultLogger::get()->debug(filename);*/


        const aiScene * scene = importer.ReadFile( filename, aiProcess_CalcTangentSpace|aiProcess_GenSmoothNormals|
                                                             aiProcess_JoinIdenticalVertices);        

        //Assimp::DefaultLogger::kill();

        if( scene )
        {
          // Get rid of old meshes, since we will want new ones.
          meshes.clear();
          for( mesh_count = 0; mesh_count < scene->mNumMeshes; ++mesh_count )
            meshes.push_back( Mesh(scene->mMeshes[mesh_count]) );

          return true;
        }
        else
        {
          return ModelError( importer.GetErrorString() );
        }

      }

      void BaseModel::QuickDraw(int i)
      {
        if( i == -1 )
        {
          for( unsigned int mesh = 0; mesh < mesh_count; ++mesh )
            meshes[mesh].QuickDraw();
        }
        else
        {
          meshes[i].QuickDraw();
        }
      }

      void BaseModel::Draw(int i)
      {
        if( i == -1 )
        {
          for( unsigned int mesh = 0; mesh < mesh_count; ++mesh )
            meshes[mesh].Draw();
        }
        else
          meshes[i].Draw();
      }

      Math::Vector3 BaseModel::BoundingCenter(unsigned int i) const
      {
        if( i > mesh_count )
        {
          ModelError( "Invalid index for BaseModel::BoundingCenter." );
          return Math::Vector3(0,0,0);
        }
        else
        {
          return meshes[i].BoundingCenter();
        }
      }

      float BaseModel::BoundingRadius(unsigned int i) const
      {
        if( i > mesh_count )
        {
          ModelError( "Invalid index for BaseModel::BoundingRadius." );
          return 1;
        }
        else
        {
          return meshes[i].BoundingRadius();
        }        
      }


      BaseModel::Mesh::~Mesh()
      {
      }

      BaseModel::Mesh::Mesh()
      {
        bounding_center = Math::Vector3(0,0,0);
        bounding_radius = 1;

        float hw = 5;
        caps.reset();
        caps.set(POSITIONS);
        caps.set(TEXCOORDS);

        for( int i = 0; i < 36; ++i )
          ib.AddIndex(i);

        vb2.Resize(36);
        vb2[0].position = Math::Vector3( -hw,-hw,hw );   //001
        vb2[1].position = Math::Vector3( -hw,-hw,-hw );  //000
        vb2[2].position = Math::Vector3( -hw,hw,-hw );   //010
        vb2[3].position = Math::Vector3( -hw,hw,-hw );   //010
        vb2[4].position = Math::Vector3( -hw,hw,hw );    //011
        vb2[5].position = Math::Vector3( -hw,-hw,hw );   //001
        vb2[0].texcoord0 = Math::Vector2(0,1);
        vb2[1].texcoord0 = Math::Vector2(1,1);
        vb2[2].texcoord0 = Math::Vector2(1,0);
        vb2[3].texcoord0 = Math::Vector2(1,0);
        vb2[4].texcoord0 = Math::Vector2(0,0);
        vb2[5].texcoord0 = Math::Vector2(0,1);

        vb2[6].position = Math::Vector3( hw,hw,hw );     //111
        vb2[7].position = Math::Vector3( hw,-hw,-hw );   //100
        vb2[8].position = Math::Vector3( hw,-hw,hw );    //101
        vb2[9].position = Math::Vector3( hw,-hw,-hw );   //100
        vb2[10].position = Math::Vector3( hw,hw,hw );    //111
        vb2[11].position = Math::Vector3( hw,hw,-hw );   //110
        vb2[6].texcoord0 = Math::Vector2(0,1);
        vb2[7].texcoord0 = Math::Vector2(1,1);
        vb2[8].texcoord0 = Math::Vector2(1,0);
        vb2[9].texcoord0 = Math::Vector2(1,0);
        vb2[10].texcoord0 = Math::Vector2(0,0);
        vb2[11].texcoord0 = Math::Vector2(0,1);

        vb2[12].position = Math::Vector3( -hw,hw,hw );   //011
        vb2[13].position = Math::Vector3( hw,hw,-hw );   //110
        vb2[14].position = Math::Vector3( hw,hw,hw );    //111
        vb2[15].position = Math::Vector3( hw,hw,-hw );   //110
        vb2[16].position = Math::Vector3( -hw,hw,hw );   //011
        vb2[17].position = Math::Vector3( -hw,hw,-hw );  //010
        vb2[12].texcoord0 = Math::Vector2(0,1);
        vb2[13].texcoord0 = Math::Vector2(1,1);
        vb2[14].texcoord0 = Math::Vector2(1,0);
        vb2[15].texcoord0 = Math::Vector2(1,0);
        vb2[16].texcoord0 = Math::Vector2(0,0);
        vb2[17].texcoord0 = Math::Vector2(0,1);

        vb2[18].position = Math::Vector3( hw,-hw,-hw );  //100
        vb2[19].position = Math::Vector3( -hw,-hw,hw );  //001
        vb2[20].position = Math::Vector3( hw,-hw,hw );   //101
        vb2[21].position = Math::Vector3( -hw,-hw,hw );  //001
        vb2[22].position = Math::Vector3( hw,-hw,-hw );  //100
        vb2[23].position = Math::Vector3( -hw,-hw,-hw ); //000
        vb2[18].texcoord0 = Math::Vector2(0,1);
        vb2[19].texcoord0 = Math::Vector2(1,1);
        vb2[20].texcoord0 = Math::Vector2(1,0);
        vb2[21].texcoord0 = Math::Vector2(1,0);
        vb2[22].texcoord0 = Math::Vector2(0,0);
        vb2[23].texcoord0 = Math::Vector2(0,1);

        vb2[24].position = Math::Vector3( -hw,hw,hw );   //011
        vb2[25].position = Math::Vector3( hw,-hw,hw );   //101
        vb2[26].position = Math::Vector3( -hw,-hw,hw );  //001
        vb2[27].position = Math::Vector3( hw,-hw,hw );   //101
        vb2[28].position = Math::Vector3( -hw,hw,hw );   //011
        vb2[29].position = Math::Vector3( hw,hw,hw );    //111
        vb2[24].texcoord0 = Math::Vector2(0,1);
        vb2[25].texcoord0 = Math::Vector2(1,1);
        vb2[26].texcoord0 = Math::Vector2(1,0);
        vb2[27].texcoord0 = Math::Vector2(1,0);
        vb2[28].texcoord0 = Math::Vector2(0,0);
        vb2[29].texcoord0 = Math::Vector2(0,1);

        vb2[30].position = Math::Vector3( hw,hw,-hw );   //110
        vb2[31].position = Math::Vector3( -hw,-hw,-hw ); //000
        vb2[32].position = Math::Vector3( hw,-hw,-hw );  //100
        vb2[33].position = Math::Vector3( -hw,-hw,-hw ); //000
        vb2[34].position = Math::Vector3( hw,hw,-hw );   //110
        vb2[35].position = Math::Vector3( -hw,hw,-hw );  //010
        vb2[30].texcoord0 = Math::Vector2(0,1);
        vb2[31].texcoord0 = Math::Vector2(1,1);
        vb2[32].texcoord0 = Math::Vector2(1,0);
        vb2[33].texcoord0 = Math::Vector2(1,0);
        vb2[34].texcoord0 = Math::Vector2(0,0);
        vb2[35].texcoord0 = Math::Vector2(0,1);
      }

      BaseModel::Mesh::Mesh( const aiMesh * mesh )
      {
        bounding_center = Math::Vector3(0,0,0);
        bounding_radius = 1;

        if( !mesh )
        {
          ModelError( "Null pointer for Mesh constructor" );
          return;
        }

        vb2.Resize(mesh->mNumVertices);
        ib.Clear();

        if( mesh->HasPositions() )
          GetPositions(mesh);

        if( mesh->HasNormals() )
          GetNormals(mesh);

        if( mesh->HasTangentsAndBitangents() )
          GetTangentsAndBitangents(mesh);

        if( mesh->HasTextureCoords(0) )
          GetTexcoords(mesh);

        if( mesh->HasVertexColors(0) )
          GetVertexColors(mesh);
      }

      int BaseModel::Mesh::VertexCount()
      {
        return vb2.Count();
      }

      const VertexBuffer3<Vertex::ModelVertex> & BaseModel::Mesh::VertexBuffer() const
      {
        return vb2;
      }

      void BaseModel::Mesh::QuickDraw()
      {
        Load();
        Draw();
        Unload();
      }

      void BaseModel::Mesh::Draw()
      {
        ib.Draw(GL_TRIANGLES);
      }

      void BaseModel::Mesh::Load()
      {
        vb2.Load();
      }

      void BaseModel::Mesh::Unload()
      {
        vb2.Unload();
      }

      void BaseModel::Mesh::GetPositions( const aiMesh * mesh )
      {
        caps.set(POSITIONS);
        int size = vb2.Count();

        Math::Vector3 minV;
        Math::Vector3 maxV;

        for( int i = 0; i < size; ++i )
        {
          vb2[i].position = Math::Vector3( mesh->mVertices[i].x,
                                          mesh->mVertices[i].y,
                                          mesh->mVertices[i].z );

          minV.x = std::min(minV.x,vb2[i].position.x);
          minV.y = std::min(minV.y,vb2[i].position.y);
          minV.z = std::min(minV.z,vb2[i].position.z);
          maxV.x = std::max(maxV.x,vb2[i].position.x);
          maxV.y = std::max(maxV.y,vb2[i].position.y);
          maxV.z = std::max(maxV.z,vb2[i].position.z);
        }

        bounding_center = 0.5f * (minV + maxV);
        bounding_radius = maxV.Magnitude();

        for( unsigned int i = 0; i < mesh->mNumFaces; ++i )
        {
          aiFace face = mesh->mFaces[i];
          for( unsigned int j = 0; j < face.mNumIndices; ++j )
          {
            ib.AddIndex( face.mIndices[j] );
          }
        }
      }

      void BaseModel::Mesh::GetNormals( const aiMesh * mesh )
      {
        caps.set(NORMALS);
        int size = vb2.Count();
        for( int i = 0; i < size; ++i )
        {
          vb2[i].normal = Math::Vector3( mesh->mNormals[i].x,
          mesh->mNormals[i].y,
          mesh->mNormals[i].z );
        }
      }

      void BaseModel::Mesh::GetTangentsAndBitangents( const aiMesh * mesh )
      {
        caps.set(TANGENTS);
        caps.set(BITANGENTS);
        int size = vb2.Count();
        for( int i = 0; i < size; ++i )
        {
          vb2[i].tangent = Math::Vector3( mesh->mTangents[i].x,
            mesh->mTangents[i].y,
            mesh->mTangents[i].z );

          vb2[i].binormal = Math::Vector3( mesh->mBitangents[i].x,
            mesh->mBitangents[i].y,
            mesh->mBitangents[i].z );
        }
      }

      void BaseModel::Mesh::GetVertexColors( const aiMesh * mesh )
      {
        caps.set(COLORS);
        int size = vb2.Count();
        for( int i = 0; i < size; ++i )
        {
          vb2[i].color = SF::Color( (unsigned char)(mesh->mColors[0][i].r*255),
          (unsigned char)(mesh->mColors[0][i].g*255),
          (unsigned char)(mesh->mColors[0][i].b*255),
          (unsigned char)(mesh->mColors[0][i].a*255) );
        }
      }

      void BaseModel::Mesh::GetTexcoords( const aiMesh * mesh )
      {
        caps.set(TEXCOORDS);
        int size = vb2.Count();
        for( int i = 0; i < size; ++i )
        {
          vb2[i].texcoord0 = Math::Vector2( mesh->mTextureCoords[0][i].x,
          1.0f-mesh->mTextureCoords[0][i].y );
        }
      }
    }
  }
}
