#pragma once
#include "WidgetImage.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace ui
{
	class WidgetButton : public WidgetImage
	{
	public:
		WidgetButton(int posX, int posY, int width, int height);
		virtual ~WidgetButton();

		// Widget interface implementation
		virtual InputEventState ProcessInput(const InputEvent& event) override;
		virtual void Draw(RenderContext& context) const override;

		// Button specific methods
		void SetText(const std::string& text);
		void SetFont(const sf::Font& font);
		void SetTextColor(const sf::Color& color);

		// Button state methods
		bool IsHovered() const;
		bool IsPressed() const;
		void SetOnClickCallback(std::function<void()> callback);

	private:
		sf::Font m_font;
		sf::Text m_text;
		bool m_isHovered;
		bool m_isPressed;
		sf::Color m_normalColor;
		sf::Color m_hoverColor;
		sf::Color m_pressedColor;
		std::function<void()> m_onClickCallback;

		void ApplicationUpdateButtonState();
		bool IsPointInside(float x, float y) const;
	};

	using WidgetButtonPtr = std::unique_ptr<WidgetButton>;
}