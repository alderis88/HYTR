#include "pch.h"
#include "WidgetButton.h"
#include <functional>

namespace ui
{
  WidgetButton::WidgetButton(int posX, int posY, int width, int height)
    : WidgetImage(posX, posY, width, height)
    , m_isHovered(false)
    , m_isPressed(false)
    , m_normalColor(sf::Color::White)
    , m_hoverColor(sf::Color(200, 200, 200))
    , m_pressedColor(sf::Color(150, 150, 150))
    , m_onClickCallback(nullptr)
  {
    // Set default text properties
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(24);

    // Position the text and sprite
    ApplicationUpdateButtonState();
  }

  WidgetButton::~WidgetButton() = default;

  InputEventState WidgetButton::ProcessInput(const InputEvent& event)
  {
    if (event.type == sf::Event::MouseMoved)
    {
      float mouseX = static_cast<float>(event.mouseMove.x);
      float mouseY = static_cast<float>(event.mouseMove.y);

      bool wasHovered = m_isHovered;
      m_isHovered = IsPointInside(mouseX, mouseY);

      if (wasHovered != m_isHovered)
      {
        ApplicationUpdateButtonState();
      }

      return m_isHovered ? InputEventState::Handled : InputEventState::Unhandled;
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        float mouseX = static_cast<float>(event.mouseButton.x);
        float mouseY = static_cast<float>(event.mouseButton.y);

        if (IsPointInside(mouseX, mouseY))
        {
          m_isPressed = true;
          ApplicationUpdateButtonState();
          return InputEventState::Handled;
        }
      }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        float mouseX = static_cast<float>(event.mouseButton.x);
        float mouseY = static_cast<float>(event.mouseButton.y);

        if (m_isPressed && IsPointInside(mouseX, mouseY))
        {
          // Button was clicked
          if (m_onClickCallback)
          {
            m_onClickCallback();
          }
          m_isPressed = false;
          ApplicationUpdateButtonState();
          return InputEventState::Handled;
        }
        else if (m_isPressed)
        {
          m_isPressed = false;
          ApplicationUpdateButtonState();
        }
      }
    }

    return InputEventState::Unhandled;
  }

  void WidgetButton::Draw(RenderContext& context) const
  {
    // Draw the image using parent WidgetImage functionality
    WidgetImage::Draw(context);

    // Always draw text on top
    context.draw(m_text);
  }



  void WidgetButton::SetText(const std::string& text)
  {
    m_text.setString(text);

    // Center the text in the button
    sf::FloatRect textBounds = m_text.getLocalBounds();
    float centerX = GetPosX() + (GetWidth() / 2.0f) - (textBounds.width / 2.0f);
    float centerY = GetPosY() + (GetHeight() / 2.0f) - (textBounds.height / 2.0f);
    m_text.setPosition(centerX, centerY);
  }

  void WidgetButton::SetFont(const sf::Font& font)
  {
    m_text.setFont(font);

    // Re-center text after font change
    if (!m_text.getString().isEmpty())
    {
      SetText(m_text.getString());
    }
  }

  void WidgetButton::SetTextColor(const sf::Color& color)
  {
    m_text.setFillColor(color);
  }



  bool WidgetButton::IsHovered() const
  {
    return m_isHovered;
  }

  bool WidgetButton::IsPressed() const
  {
    return m_isPressed;
  }

  void WidgetButton::SetOnClickCallback(std::function<void()> callback)
  {
    m_onClickCallback = callback;
  }

  void WidgetButton::ApplicationUpdateButtonState()
  {
    if (m_imageLoadStockProductsed)
    {
      sf::Color modulation = sf::Color::White;

      if (m_isPressed)
        modulation = m_pressedColor;
      else if (m_isHovered)
        modulation = m_hoverColor;
      else
        modulation = m_normalColor;

      m_sprite.setColor(modulation);
    }
  }

  bool WidgetButton::IsPointInside(float x, float y) const
  {
    return (x >= GetPosX() && x <= GetPosX() + GetWidth() &&
        y >= GetPosY() && y <= GetPosY() + GetHeight());
  }
}