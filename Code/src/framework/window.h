#pragma once

#include "widget.h"

namespace ui
{
  class Window final
  {
  public:
    Window();
    ~Window();

    void ProcessInput( const InputEvent& event );
    void Draw( RenderContext& context ) const;

    void AddWidget( WidgetPtr&& widget );

  private:
    std::vector< WidgetPtr > m_widgets;

  };
}
