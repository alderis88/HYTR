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
    int GetPosRelX() const;
    int GetPosRelY() const;
    int GetPosAbsX() const;
    int GetPosAbsY() const;
    int GetWidth() const;
    int GetHeight() const;

    // Position and size setters
    virtual void SetPosRelX(int posRelX);
    virtual void SetPosRelY(int posRelY);
    virtual void SetPosAbsX(int posAbsX);
    virtual void SetPosAbsY(int posAbsY);
    virtual void SetWidth(int width);
    virtual void SetHeight(int height);

    // Visibility control
    void SetVisible(bool visible);
    bool IsVisible() const;

    // Position update notification - called after position changes
    virtual void UpdatePosition() {}

  protected:

  private:
    int m_posRelX;   // Relative position within parent container
    int m_posRelY;   // Relative position within parent container
    int m_posAbsX;   // Absolute position on screen
    int m_posAbsY;   // Absolute position on screen
    int m_width;
    int m_height;
    bool m_visible = true; // Default visible
  };

  using WidgetPtr = std::unique_ptr< Widget >;
}

