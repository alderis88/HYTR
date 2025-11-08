#include "pch.h"
#include "widget.h"
#include "WidgetContainer.h"

namespace ui
{
	Widget::Widget( int posX, int posY, int width, int height )
		: m_relPosX( posX )
		, m_relPosY( posY )
		, m_absPosX( posX ) // No parent yet, absolute == relative
		, m_absPosY( posY )
		, m_width( width )
		, m_height( height )
	{
	}

	Widget::~Widget() = default;

	int Widget::GetPosX() const { return m_relPosX; }
	int Widget::GetPosY() const { return m_relPosY; }

	int Widget::GetWidth() const
	{
		return m_width;
	}

	int Widget::GetHeight() const
	{
		return m_height;
	}

	void Widget::SetPosX(int posX)
	{
		m_relPosX = posX;
		RecomputeAbsolutePosition();
	}

	void Widget::SetPosY(int posY)
	{
		m_relPosY = posY;
		RecomputeAbsolutePosition();
	}

	void Widget::SetWidth(int width)
	{
		m_width = width;
	}

	void Widget::SetHeight(int height)
	{
		m_height = height;
	}

	void Widget::SetParent(WidgetContainer* parent)
	{
		m_parent = parent;
		RecomputeAbsolutePosition();
	}

	int Widget::GetAbsolutePosX() const { return m_absPosX; }
	int Widget::GetAbsolutePosY() const { return m_absPosY; }

	void Widget::RecomputeAbsolutePosition()
	{
		if (m_parent)
		{
			m_absPosX = m_parent->GetAbsolutePosX() + m_relPosX;
			m_absPosY = m_parent->GetAbsolutePosY() + m_relPosY;
		}
		else
		{
			m_absPosX = m_relPosX;
			m_absPosY = m_relPosY;
		}
		OnAbsolutePositionChanged();
	}

	void Widget::OnAbsolutePositionChanged()
	{
		// Default: no-op. Derived widgets override if they need to update internal drawable positions.
	}

	void Widget::EnableDebugDraw(bool enable, sf::Color color)
	{
		m_debugDraw = enable;
		m_debugColor = color;
	}

	void Widget::DrawDebugBounds(RenderContext& context) const
	{
		if (!m_debugDraw) return;
		sf::RectangleShape rect(sf::Vector2f(static_cast<float>(GetWidth()), static_cast<float>(GetHeight())));
		rect.setPosition(static_cast<float>(GetAbsolutePosX()), static_cast<float>(GetAbsolutePosY()));
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(m_debugColor);
		rect.setOutlineThickness(1.0f);
		context.draw(rect);
	}

}
