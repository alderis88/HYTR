#include "pch.h"
#include "widget.h"

namespace ui
{
  Widget::Widget( int posX, int posY, int width, int height )
    : m_posRelX( posX )
    , m_posRelY( posY )
    , m_posAbsX( posX )
    , m_posAbsY( posY )
    , m_width( width )
    , m_height( height )
    , m_visible( true )
  {
  }

  Widget::~Widget() = default;

  int Widget::GetPosRelX() const
  {
    return m_posRelX;
  }

  int Widget::GetPosRelY() const
  {
    return m_posRelY;
  }

  int Widget::GetPosAbsX() const
  {
    return m_posAbsX;
  }

  int Widget::GetPosAbsY() const
  {
    return m_posAbsY;
  }

  int Widget::GetWidth() const
  {
    return m_width;
  }

  int Widget::GetHeight() const
  {
    return m_height;
  }

  void Widget::SetPosRelX(int posRelX)
  {
    m_posRelX = posRelX;
  }

  void Widget::SetPosRelY(int posRelY)
  {
    m_posRelY = posRelY;
  }

  void Widget::SetPosAbsX(int posAbsX)
  {
    m_posAbsX = posAbsX;
  }

  void Widget::SetPosAbsY(int posAbsY)
  {
    m_posAbsY = posAbsY;
  }

  void Widget::SetWidth(int width)
  {
    m_width = width;
  }

  void Widget::SetHeight(int height)
  {
    m_height = height;
  }

  void Widget::SetVisible(bool visible)
  {
    m_visible = visible;
  }

  bool Widget::IsVisible() const
  {
    return m_visible;
  }

}
