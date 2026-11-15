/***** A  ll content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CInputRegion.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/13/2009 @ 13:03:53 - nwilliam
**  \brief   UI text box implementation.
*******************************************************************************/

#include "CInputRegion.h"
#include "MenuSystem.h"
#include "Utilities/Misc.h"
#include "Engine/InputEngine.h"
#include "Script/ScriptEngine.h"
#include "SolarFlare/Globals.h" // menu, input

#include "Utilities/LeakCheck.h"

namespace SF {
namespace UI
{
  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  CInputRegion::CInputRegion() : m_cursorOffset( 0 ),
                                 m_textComp( 0 ),
                                 m_maxLength( 10 ),
                                 m_cursorActive( false )
  {
  }

  CInputRegion::~CInputRegion()
  {
  }

  Engine::BaseComponent* CInputRegion::Clone() const
  {
    return new CInputRegion( *this );
  }

  void CInputRegion::PushToLua( lua_State *ls )
  {
    Lunar<CInputRegion>::push( ls, this );
  }

  void CInputRegion::Focus()
  {
    InsertCursor();
    CRegion::Focus();
  }

  void CInputRegion::Unfocus()
  {
    RemoveCursor();
    CRegion::Unfocus();
  }

  void CInputRegion::OnKeypress()
  {
    using namespace Engine;
    using namespace InputMap;

    if( !m_textComp )
    {
      return;
    }

    // A single key press will enter this function upwards of a dozen times
    // for normal typing. This is to avoid calling into lua with no update
    bool doCallback = false;

    std::string content = m_textComp->GetText();
    content.erase( m_cursorOffset, 1 );
    
    if( Globals::input->IsRepeating( BACKSPACE ) )
    {
      if( content.length() != 0  && m_cursorOffset > 0 )
      {
        doCallback = true;
        content.erase( m_cursorOffset - 1, 1 );
        --m_cursorOffset;
      }
    }
    else if( Globals::input->IsRepeating( DELETE ) )
    {
      if( content.length() != 0 && m_cursorOffset < content.length() )
      {
        doCallback = true;
        content.erase( m_cursorOffset, 1 );
      }
    }
    else if( Globals::input->IsRepeating( ARROW_LEFT ) )
    {
      if( m_cursorOffset > 0 )
      {
        --m_cursorOffset;
      }
    }
    else if( Globals::input->IsRepeating( ARROW_RIGHT ) )
    {
      m_cursorOffset = Utilities::Min( m_cursorOffset + 1, content.length() );
    }
    else if( Globals::input->IsRepeating( ENTER ) || Globals::input->IsRepeating( NUMPAD_ENTER ) )
    {
      Unfocus();
      return;
    }
    else
    {
      const std::string& newText = Globals::input->TextInput();
      size_t len = newText.length();

      if( len && content.length() < m_maxLength )
      {
        doCallback = true;
        content.insert( m_cursorOffset, newText );
        m_cursorOffset += len;
      }
    }

    if( doCallback )
    {
      DoCallback( content );
    }

    content.insert( m_cursorOffset, "|" );
    m_textComp->SetText( content );
  }

  void CInputRegion::OnMouseOut()
  {
    m_mouseOver = false;
  }

  void CInputRegion::OnMouseOver()
  {
    m_mouseOver = true;
  }

  void CInputRegion::OnLeftClick()
  {
    if( m_mouseOver )
    {
      Focus();
      Globals::menuSystem->ProcessedClick( true );
    }
    else
    {
      Unfocus();
    }
  }

  bool CInputRegion::InitDependencies( Engine::Entity* e )
  {
    return CBasicElement::InitDependencies( e );
  }

  Engine::BaseComponent* CInputRegion::Creator( const ticpp::Element* )
  {
    return new CInputRegion;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private Below
  //////////////////////////////////////////////////////////////////////////////
  void CInputRegion::InsertCursor()
  {
    if( m_textComp && !m_cursorActive )
    {
      m_cursorActive = true;
      std::string content = m_textComp->GetText();
      m_cursorOffset = content.length();

      content += "|";
      m_textComp->SetText( content );
    }
  }

  void CInputRegion::RemoveCursor()
  {
    if( m_textComp && m_cursorActive )
    {
      m_cursorActive = false;

      std::string content = m_textComp->GetText();
      content.erase( m_cursorOffset, 1 );
      m_textComp->SetText( content );
    }
  }

  void CInputRegion::DoCallback( const std::string& content )
  {
    if( m_keyPressCallback )
    {
      lua_State* ls = Globals::scriptEngine->MasterState();
      lua_pushlstring( ls, content.c_str(), content.length() );
      m_keyPressCallback( ls, 1 );
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua bound methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( CInputRegion )
    LUA_NAME_METHOD( CInputRegion, "Base",          LBase )
    LUA_NAME_METHOD( CInputRegion, "Region",        LRegion )
    LUA_NAME_METHOD( CInputRegion, "Content",       LContent )
    LUA_NAME_METHOD( CInputRegion, "MaxLength",     LMaxLength )
    LUA_NAME_METHOD( CInputRegion, "OnKeyPress",    LOnKeyPress )
    LUA_NAME_METHOD( CInputRegion, "TextComponent", LTextComponent )
    

    // From CRegion
    LUA_NAME_METHOD( CInputRegion, "Orphan",          LOrphan )
    LUA_NAME_METHOD( CInputRegion, "OrphanTransform", LOrphanTransform )
    LUA_NAME_METHOD( CInputRegion, "SetColor",        LSetColor )
    LUA_NAME_METHOD( CInputRegion, "GetColor",        LGetColor )
    LUA_NAME_METHOD( CInputRegion, "SetAlpha",        LSetAlpha )
    LUA_NAME_METHOD( CInputRegion, "GetAlpha",        LGetAlpha )
    LUA_NAME_METHOD( CInputRegion, "SetAlignment",    LSetAlignment )
    LUA_NAME_METHOD( CInputRegion, "SetLayout",       LSetLayout )
    LUA_NAME_METHOD( CInputRegion, "SetMargin",       LSetMargin )
    LUA_NAME_METHOD( CInputRegion, "SetPadding",      LSetPadding )
    LUA_NAME_METHOD( CInputRegion, "SetPosition",     LSetPosition )
    LUA_NAME_METHOD( CInputRegion, "SetPositionMode", LSetPositionMode )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD( CInputRegion, LBase )
  {
    CBasicElement::PushToLua( ls );
    return 1;
  }

  LUA_DEFINE_METHOD( CInputRegion, LRegion )
  {
    CRegion::PushToLua( ls );
    return 1;
  }

  LUA_DEFINE_METHOD( CInputRegion, LContent )
  {
    if( !m_textComp )
    {
      luaL_error( ls, "No text component set" );
    }

    if( lua_gettop( ls ) == 1 )
    {
      const char* str = luaL_checkstring( ls, 1 );

      std::string content = str;
      m_cursorOffset = content.length();
      m_textComp->SetText( content );
      
      DoCallback( content );

      if( m_focus )
      {
        InsertCursor();
      }

      return 0;
    }

    std::string content = m_textComp->GetText();

    if( m_focus )
    {
      content.erase( m_cursorOffset, 1 );
    }

    lua_pushlstring( ls, content.c_str(), content.length() );
    return 1;
  }

  LUA_DEFINE_METHOD( CInputRegion, LMaxLength )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushnumber( ls, m_maxLength );
      return 1;
    }

    size_t newSize = static_cast<size_t>( luaL_checkint( ls, 1 ) );
    m_maxLength = newSize;

    return 0;
  }

  LUA_DEFINE_METHOD( CInputRegion, LOnKeyPress )
  {
    if( lua_gettop( ls ) == 1 )
    {
      m_keyPressCallback.Set( ls );
      return 0;
    }

    m_keyPressCallback.Get( ls );
    return 1;
  }

  LUA_DEFINE_METHOD( CInputRegion, LTextComponent )
  {
    m_textComp = Lunar<Graphics::Components::CText>::check( ls, 1 );
    return 0;
  }

  #define INHERITED_LUA_METHOD(x) LUA_DEFINE_METHOD( CInputRegion, x ) { return CRegion::x(ls); }

  INHERITED_LUA_METHOD( LOrphan )
  INHERITED_LUA_METHOD( LOrphanTransform )
  INHERITED_LUA_METHOD( LSetColor )
  INHERITED_LUA_METHOD( LGetColor )
  INHERITED_LUA_METHOD( LSetAlpha )
  INHERITED_LUA_METHOD( LGetAlpha )
  INHERITED_LUA_METHOD( LSetAlignment )
  INHERITED_LUA_METHOD( LSetLayout )
  INHERITED_LUA_METHOD( LSetMargin )
  INHERITED_LUA_METHOD( LSetPadding )
  INHERITED_LUA_METHOD( LSetPosition )
  INHERITED_LUA_METHOD( LSetPositionMode )

  #undef INHERITED_LUA_METHOD

} // UI namespace
} // SF namespace
