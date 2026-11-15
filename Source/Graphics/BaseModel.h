/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseModel.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 10/04/2008 @ 11:47 - CJT 
**  \brief 
*******************************************************************************/

#ifndef SF_BASE_MODEL_H_
#define SF_BASE_MODEL_H_

#include <string>
#include <vector>
#include <bitset>
#include "Utilities/SharedPtr.h"





#include "VertexBuffer3.h"
#include "IndexBuffer.h"

namespace Assimp
{
	class Importer;
}

namespace Vertex
{
  class ModelVertex;
}

struct aiScene;
struct aiMesh;

namespace SF { namespace Engine { namespace Assets { struct ModelAsset; } } }


namespace SF
{
  namespace Graphics
  {
    namespace Model
    {
      class BaseModel
      {
        class Mesh;
      public:
        BaseModel();
        ~BaseModel();

        bool LoadFromFile( const std::string & filename );
        void QuickDraw(int i = -1);
        void Draw(int i = -1);
        void Load(int i = -1);
        void Unload(int i = -1);

        Math::Vector3 BoundingCenter(unsigned int i = 0) const;
        float BoundingRadius(unsigned int i = 0) const;

        int VertexCount(int i = -1);
        const VertexBuffer3<Vertex::ModelVertex> * VertexBuffer( int i = 0 ) const;

      private:
        void Clear();
        std::vector<Mesh> meshes;
        unsigned int mesh_count;

        

      };

      class BaseModel::Mesh
      {
        enum { POSITIONS, NORMALS, TANGENTS, BITANGENTS, TEXCOORDS, COLORS };
      public:

        Mesh(); // Default cube object
        ~Mesh();

        Mesh( const aiMesh * mesh ); // Load from assimp
        
        void QuickDraw();
        void Draw();
        void Load();
        void Unload();
        int VertexCount();

        Math::Vector3 BoundingCenter() const { return bounding_center; }
        float BoundingRadius() const { return bounding_radius; }

        const VertexBuffer3<Vertex::ModelVertex> & VertexBuffer() const;

      private:


        void GetPositions( const aiMesh * mesh );
        void GetNormals( const aiMesh * mesh );
        void GetTangentsAndBitangents( const aiMesh * mesh );
        void GetVertexColors( const aiMesh * mesh );
        void GetTexcoords( const aiMesh * mesh );
        std::bitset<12> caps;


        VertexBuffer3<Vertex::ModelVertex> vb2;
        IndexBuffer ib;

        float bounding_radius;
        Math::Vector3 bounding_center;
      };
    }
    
    typedef Utilities::SharedPtr< Model::BaseModel > SharedModel;
  }
}

#endif
