#pragma once
#include "widget.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace ui
{
  class WidgetText : public Widget
  {
  public:
    WidgetText(int posX, int posY, const std::string& text = "");
    virtual ~WidgetText();

    // Widget interface implementation
    virtual InputEventState ProcessInput(const InputEvent& event) override;
    virtual void Draw(RenderContext& context) const override;

    // Text specific methods
    void SetText(const std::string& text);
    void SetFont(const sf::Font& font);
    void SetTextColor(const sf::Color& color);
    void SetCharacterSize(unsigned int size);
    void SetStyle(sf::Uint32 style);
    void SetPosition(float x, float y);

    // Text alignment
    enum class Alignment
    {
      Left,
      Center,
      Right
    };
    void SetAlignment(Alignment alignment);

    // Getters
    const std::string& GetText() const;
    sf::Color GetTextColor() const;
    unsigned int GetCharacterSize() const;

    // Override position update notification
    void UpdatePosition() override;

  private:
    sf::Text m_text;
    sf::Font m_defaultFont;
    bool m_hasCustomFont;
    Alignment m_alignment;
    std::string m_textString;

    void ApplicationUpdateTextPosition();
  };

  using WidgetTextPtr = std::unique_ptr<WidgetText>;
}