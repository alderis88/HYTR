#pragma once
#include "pch.h"
#include <memory>

namespace ui
{
  class Widget
  {
  public:
    Widget( int posX, int posY, int width, int height );
    virtual ~Widget();

    virtual InputEventState ProcessInput( const InputEvent& event ) = 0;
    virtual void Draw( RenderContext& context ) const = 0;

    // Position and size getters
    int GetPosX() const;
    int GetPosY() const;
    int GetWidth() const;
    int GetHeight() const;

    // Position and size setters
    virtual void SetPosX(int posX);
    virtual void SetPosY(int posY);
    virtual void SetWidth(int width);
    virtual void SetHeight(int height);

  protected:

  private:
    int m_posX;
    int m_posY;
    int m_width;
    int m_height;
  };

  using WidgetPtr = std::unique_ptr< Widget >;
}

