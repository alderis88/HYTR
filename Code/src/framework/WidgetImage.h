#pragma once
#include "widget.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace ui
{
  class WidgetImage : public Widget
  {
  public:
    WidgetImage(int posX, int posY, int width, int height, const std::string& imagePath = "");
    virtual ~WidgetImage();

    // Widget interface implementation
    virtual InputEventState ProcessInput(const InputEvent& event) override;
    virtual void Draw(RenderContext& context) const override;

    // Override position setters to update sprite position
    virtual void SetPosX(int posX) override;
    virtual void SetPosY(int posY) override;

    // Image specific methods
    bool LoadImage(const std::string& imagePath);
    void SetPosition(float x, float y);
    void SetScale(float scaleX, float scaleY);

  protected:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    bool m_imageLoadStockProductsed;
  };

  using WidgetImagePtr = std::unique_ptr<WidgetImage>;
}