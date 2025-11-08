#pragma once
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

	protected:
		int GetPosX() const;
		int GetPosY() const;

		int GetWidth() const;
		int GetHeight() const;

		void SetPosX(int posX);
		void SetPosY(int posY);
		void SetWidth(int width);
		void SetHeight(int height);

	private:
		int m_posX;
		int m_posY;
		int m_width;
		int m_height;
	};

	using WidgetPtr = std::unique_ptr< Widget >;
}

