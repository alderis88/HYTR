#include "pch.h"
#include "WidgetImage.h"
#include "../application/application.h"

namespace ui
{
  WidgetImage::WidgetImage(int posX, int posY, int width, int height, const std::string& imagePath)
    : Widget(posX, posY, width, height)
    , m_imageLoadStockProductsed(false)
  {
    // LoadStockProducts image if filename is provided
    if (!imagePath.empty())
    {
      LoadImage(imagePath);
    }
  }

  WidgetImage::~WidgetImage() = default;

  InputEventState WidgetImage::ProcessInput(const InputEvent& event)
  {
    // Image widgets don't typically handle input events
    // Return Unhandled so parent can process the event
    return InputEventState::Unhandled;
  }

  void WidgetImage::Draw(RenderContext& context) const
  {
    // Only draw if widget is visible
    if (IsVisible() && m_imageLoadStockProductsed)
    {
      context.draw(m_sprite);
    }
  }

  bool WidgetImage::LoadImage(const std::string& imagePath)
  {
    // Use Application's static assets path directly
    std::string fullPath = Application::s_assetsPath + imagePath;

    if (m_texture.loadFromFile(fullPath))
    {
      m_sprite.setTexture(m_texture);

      // Set the position based on widget coordinates
      m_sprite.setPosition(static_cast<float>(GetPosAbsX()), static_cast<float>(GetPosAbsY()));

      // Scale the image to fit the widget dimensions if needed
      sf::Vector2u textureSize = m_texture.getSize();
      float scaleX = static_cast<float>(GetWidth()) / textureSize.x;
      float scaleY = static_cast<float>(GetHeight()) / textureSize.y;
      m_sprite.setScale(scaleX, scaleY);

      m_imageLoadStockProductsed = true;
      return true;
    }
    else
    {
      m_imageLoadStockProductsed = false;
      return false;
    }
  }

  void WidgetImage::SetPosition(float x, float y)
  {
    m_sprite.setPosition(x, y);
  }

  void WidgetImage::SetScale(float scaleX, float scaleY)
  {
    m_sprite.setScale(scaleX, scaleY);
  }

  void WidgetImage::UpdatePosition()
  {
    if (m_imageLoadStockProductsed)
    {
      m_sprite.setPosition(static_cast<float>(GetPosAbsX()), static_cast<float>(GetPosAbsY()));
    }
  }
}