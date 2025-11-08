#include "pch.h"
#include "WidgetText.h"
#include "../application/application.h"

namespace ui
{
	WidgetText::WidgetText(int posX, int posY, const std::string& text)
		: Widget(posX, posY, 0, 0)  // Width and height will be calculated based on text
		, m_hasCustomFont(false)
		, m_alignment(Alignment::Left)
		, m_textString(text)
	{
		std::string fullPath = Application::s_assetsPath + "FontBasic.ttf";

		// LoadStockProducts default font from assets
		if (m_defaultFont.loadFromFile(fullPath))
		{
			m_text.setFont(m_defaultFont);
		}

		// Set default text properties
		m_text.setFillColor(sf::Color::White);
		m_text.setCharacterSize(24);
		m_text.setStyle(sf::Text::Regular);

		// Set the text and ApplicationUpdate position
		SetText(text);
	}

	WidgetText::~WidgetText() = default;

	InputEventState WidgetText::ProcessInput(const InputEvent& event)
	{
		// Text widgets typically don't handle input events
		// They are just for display purposes
		return InputEventState::Unhandled;
	}

	void WidgetText::Draw(RenderContext& context) const
	{
		context.draw(m_text);
	}

	void WidgetText::SetText(const std::string& text)
	{
		m_textString = text;
		m_text.setString(text);

		// ApplicationUpdate widget dimensions based on text bounds
		sf::FloatRect textBounds = m_text.getLocalBounds();
		SetWidth(static_cast<int>(textBounds.width));
		SetHeight(static_cast<int>(textBounds.height));

		// ApplicationUpdate position based on alignment
		ApplicationUpdateTextPosition();
	}

	void WidgetText::SetFont(const sf::Font& font)
	{
		m_text.setFont(font);
		m_hasCustomFont = true;

		// Recalculate dimensions and position after font change
		if (!m_textString.empty())
		{
			SetText(m_textString);
		}
	}

	void WidgetText::SetTextColor(const sf::Color& color)
	{
		m_text.setFillColor(color);
	}

	void WidgetText::SetCharacterSize(unsigned int size)
	{
		m_text.setCharacterSize(size);

		// Recalculate dimensions and position after size change
		if (!m_textString.empty())
		{
			SetText(m_textString);
		}
	}

	void WidgetText::SetStyle(sf::Uint32 style)
	{
		m_text.setStyle(style);

		// Recalculate dimensions and position after style change
		if (!m_textString.empty())
		{
			SetText(m_textString);
		}
	}
	
	void WidgetText::SetPosition(float x, float y)
	{
		// ApplicationUpdate widget position
		SetPosX(static_cast<int>(x));
		SetPosY(static_cast<int>(y));

		// ApplicationUpdate text position based on alignment
		ApplicationUpdateTextPosition();
	}

	void WidgetText::SetAlignment(Alignment alignment)
	{
		m_alignment = alignment;
		ApplicationUpdateTextPosition();
	}

	const std::string& WidgetText::GetText() const
	{
		return m_textString;
	}

	sf::Color WidgetText::GetTextColor() const
	{
		return m_text.getFillColor();
	}

	unsigned int WidgetText::GetCharacterSize() const
	{
		return m_text.getCharacterSize();
	}

	void WidgetText::ApplicationUpdateTextPosition()
	{
		sf::FloatRect textBounds = m_text.getLocalBounds();
		float posX = static_cast<float>(GetPosX());
		float posY = static_cast<float>(GetPosY());

		// Apply alignment
		switch (m_alignment)
		{
		case Alignment::Left:
			// Default position (no offset needed)
			break;

		case Alignment::Center:
			posX -= textBounds.width / 2.0f;
			break;

		case Alignment::Right:
			posX -= textBounds.width;
			break;
		}

		// Account for local bounds offset (SFML quirk)
		posX -= textBounds.left;
		posY -= textBounds.top;

		m_text.setPosition(posX, posY);
	}


}