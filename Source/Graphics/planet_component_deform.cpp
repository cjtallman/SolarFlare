/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    planet_component_deform.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:31:46 CTallman
**  \brief   Deforms the planet mesh.
*******************************************************************************/

#include "CPlanetGraphic.h"
#include "SolarFlare/Globals.h"
#include "Threading/TaskManager.h"
#include "Engine/AssetManager.h"
#include "Math/TMatrix4x4.h"
#include "Utilities/LeakCheck.h"
#include "StateManager.h"
namespace SF
{
  namespace Graphics
  {
    namespace Components
    {

      void ChooseThreeFaces( const Vector3 & input, int & face0, int & face1, int & face2 )
      {
        float dx = input.x;
        float dy = input.y;
        float dz = input.z;
        face0 = dx > -dx ? 0 : 1;
        face1 = dy > -dy ? 2 : 3;
        face2 = dz > -dz ? 4 : 5;
      }

      void CPlanetGraphic::AddDeform( const Math::Vector3 & position, float scale )
      {
        Math::Quaternion quat = Math::Quaternion::AlignToVector( position.Normalized(), Math::Vector3(0,1,0));

        Vector3 X = scale * quat.LocalX().Normalized();
        Vector3 Y = 20 * quat.LocalY().Normalized();
        Vector3 Z = scale * quat.LocalZ().Normalized();

        deform_scales.push_back(scale);

        int faces[3];
        ChooseThreeFaces(Y, faces[0], faces[1], faces[2]);
        deform_faces |= (1 << faces[0]);
        deform_faces |= (1 << faces[1]);
        deform_faces |= (1 << faces[2]);

        Vector3 p0(Y + X - Z);
        Vector3 p1(Y + X + Z);
        Vector3 p2(Y - X + Z);
        Vector3 p3(Y - X - Z);
        Vector2 t0(0,0);
        Vector2 t1(1,0);
        Vector2 t2(1,1);
        Vector2 t3(0,1);
        Vertex::DeformVertex vtx;
        vtx.position = p0;
        vtx.texcoord0 = t0;
        vtx.scale = (scale);
        deform_verts.AddVertex(vtx);
        vtx.position = p1;
        vtx.texcoord0 = t1;
        deform_verts.AddVertex(vtx);
        vtx.position = p2;
        vtx.texcoord0 = t2;
        deform_verts.AddVertex(vtx);
        vtx.position = p3;
        vtx.texcoord0 = t3;
        deform_verts.AddVertex(vtx);

      }

      void CPlanetGraphic::RenderDeform()
      {
        glMatrixMode(GL_PROJECTION);
        PushMatrix();
        glLoadMatrixf( SquarePerspectiveMatrix().Array() );
        glMatrixMode(GL_MODELVIEW);
        PushMatrix();
        glViewport(0,0,deform_texture.Width(), deform_texture.Width());
        //                    X     -X    Y     -Y    Z     -Z
        float rotate_x[6] = { 180,  180,  -90,  90,   180,  180 };
        float rotate_y[6] = { -90,  90,   0,    0,    0,    180 };

        crater_texture->Bind(0);
        deform_verts.Load();

        
        for( int i = 0; i < 6; ++i )
        {
          if( deform_faces & (1<<i) )
          {
            deform_texture.BeginCapture(i);
            glLoadIdentity();
            glRotatef( rotate_x[i], 1, 0, 0);
            glRotatef( rotate_y[i], 0, 1, 0);

            deform_verts.Draw(GL_QUADS);
            deform_texture.UpdateLocalCopy(i);
          }
        }
        deform_verts.Unload();
        deform_verts.Clear();
        deform_texture.EndCapture();
        crater_texture->Unbind();
        deform_faces = 0;

        glMatrixMode(GL_PROJECTION);
        PopMatrix();
        glMatrixMode(GL_MODELVIEW);
        PopMatrix();
      }

      void CPlanetGraphic::Deform()
      {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        glBlendFunc( GL_DST_COLOR, GL_ZERO );
        glEnable(GL_BLEND);

        if( deform_verts.Count() )
        {
          Performance::StateManager::SetActiveFragmentShader(deformFrag.Get());
          RenderDeform();
          updated_heights = false;
          Performance::StateManager::SetActiveFragmentShader(0);
        }

        glPopAttrib();
      }


      struct DeformParams
      {
        DeformParams( CPlanetGraphic * sun )
          : component(sun) {}

        CPlanetGraphic * component;
      };

      void DeformWrapper( const DeformParams & params )
      {
        const VertexBuffer3<Vertex::ModelVertex> * mesh = params.component->GetModel().Get()->VertexBuffer(0);
        VertexBuffer3<Vertex::HeightVertex> * temp = params.component->GetBackHeightBuffer();
        TextureType::Texture2D * tex = params.component->GetHeightTexture();
        TextureType::CubemapTexture * cube = params.component->GetDeformTexture();

        if( mesh )
        {
          int count = mesh->Count();
          if( temp->Count() != mesh->Count() )
            temp->Resize(count);

          for( int i = 0; i < count; ++i )
          {
            Color4f deform_color = (Color4f)cube->GetTexel( (*mesh)[i].position );
            Color4f noise_color = tex->GetTexel((*mesh)[i].texcoord0.x,(*mesh)[i].texcoord0.y);
            (*temp)[i].displacement = deform_color.r * noise_color.r;
          }
        }
      }

      void CPlanetGraphic::SwapHeightBuffers()
      {
        VertexBuffer3<Vertex::HeightVertex> * temp = back_height_buffer;
        back_height_buffer = height_buffer;
        height_buffer = temp;
      }

      void CPlanetGraphic::UpdateHeightBuffer()
      {
        updated_heights = true;

        unsigned int lastTask = 0;
        if( !m_taskList.empty() )
          lastTask = m_taskList.back();

        DeformParams params( this );

        unsigned int taskID = Globals::taskManager->AddTask(
          DeformWrapper,
          params,
          Engine::SwapCallback::Method<CPlanetGraphic, &CPlanetGraphic::SwapHeightBuffers >(this),
          false,
          lastTask );

        m_taskList.push_back( taskID );

        // Remove finished taskIDs
        for( std::list<unsigned int>::iterator iter = m_taskList.begin(); iter != m_taskList.end();)
        {
          if( !Globals::taskManager->IsValidTaskID( *iter ) )
            m_taskList.erase( iter++ );
          else
            ++iter;
        }
      }


    }
  }
}