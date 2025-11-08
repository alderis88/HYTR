#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

namespace ui
{
	class WidgetContainer; // Forward declaration

	class Widget
	{
	public:
			Widget( int posX, int posY, int width, int height );
		virtual ~Widget();

		virtual InputEventState ProcessInput( const InputEvent& event ) = 0;
		virtual void Draw( RenderContext& context ) const = 0;

			// Relative position getters (within parent container)
			int GetPosX() const; // Relative X
			int GetPosY() const; // Relative Y
		int GetWidth() const;
		int GetHeight() const;

			// Absolute position getters (window coordinates)
			int GetAbsolutePosX() const;
			int GetAbsolutePosY() const;

		// Position and size setters
			void SetPosX(int posX);
			void SetPosY(int posY);
		void SetWidth(int width);
		void SetHeight(int height);

		// Parent management
		void SetParent(WidgetContainer* parent);
			WidgetContainer* GetParent() const { return m_parent; }
			// Recompute absolute position from parent (used by containers after layout changes)
			void RecomputeAbsolutePosition();

			// Hook: derived widgets can react when absolute position changes (e.g. update SFML sprite/text)
			virtual void OnAbsolutePositionChanged();

			// Debug draw support (now common to all widgets)
			void EnableDebugDraw(bool enable, sf::Color color = sf::Color(255,0,0,80));
			bool IsDebugDrawEnabled() const { return m_debugDraw; }
			sf::Color GetDebugColor() const { return m_debugColor; }

		protected:
			// Common debug draw rectangle helper
			void DrawDebugBounds(RenderContext& context) const;

		private:
			int m_relPosX;    // Relative position X in container
			int m_relPosY;    // Relative position Y in container
			int m_absPosX;    // Absolute position X in window
			int m_absPosY;    // Absolute position Y in window
		int m_width;
		int m_height;
		WidgetContainer* m_parent = nullptr;
				bool m_debugDraw = false;
				sf::Color m_debugColor = sf::Color(255,0,0,80);
	};

	using WidgetPtr = std::unique_ptr< Widget >;
}

