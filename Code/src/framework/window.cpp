#include "pch.h"
#include "window.h"

namespace ui
{
	Window::Window() = default;
	Window::~Window() = default;

	void Window::ProcessInput( const InputEvent& event )
	{
		for( auto& widget : m_widgets )
		{
			if( widget->ProcessInput( event ) == InputEventState::Handled )
			{
				return;
			}
		}
	}

	void Window::Draw( RenderContext& context ) const
	{
		for( auto& widget : m_widgets )
		{
			widget->Draw( context );
		}
	}

	void Window::AddWidget( WidgetPtr&& widget )
	{
		m_widgets.push_back( std::move( widget ) );
	}

}
