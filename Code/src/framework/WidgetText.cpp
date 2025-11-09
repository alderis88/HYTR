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
    , m_fadingEnabled(false)
    , m_fadeDirection(true)
    , m_fadingSpeed(255.0f)  // Default: 255 alpha/second (1 second for full fade)
    , m_currentAlpha(255.0f)
    , m_originalColor(sf::Color::White)
  {
    std::string fullPath = Application::s_assetsPath + "FontBasic.ttf";

    // LoadStockProducts default font from assets
    if (m_defaultFont.loadFromFile(fullPath))
    {
      m_text.setFont(m_defaultFont);
    }

    // Set default text properties
    m_originalColor = sf::Color::White;
    m_text.setFillColor(m_originalColor);
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
    // Only draw if we have a valid font and widget is visible
    if (m_text.getFont() != nullptr && IsVisible())
    {
      context.draw(m_text);
    }
  }

  void WidgetText::Update(float deltaTime)
  {
    if (m_fadingEnabled)
    {
      // Calculate alpha change based on speed and time
      float alphaChange = m_fadingSpeed * deltaTime;

      if (m_fadeDirection)
      {
        // Fade in
        m_currentAlpha += alphaChange;
        if (m_currentAlpha >= 255.0f)
        {
          m_currentAlpha = 255.0f;
          m_fadingEnabled = false; // Stop fading when fully visible
        }
      }
      else
      {
        // Fade out
        m_currentAlpha -= alphaChange;
        if (m_currentAlpha <= 0.0f)
        {
          m_currentAlpha = 0.0f;
          m_fadingEnabled = false; // Stop fading when fully transparent
        }
      }

      // Update the text alpha
      UpdateTextAlpha();
    }
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
    m_originalColor = color;
    m_text.setFillColor(color);
    // Update current alpha to match new color
    m_currentAlpha = static_cast<float>(color.a);
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
    // ApplicationUpdate widget position - now using absolute position directly
    SetPosAbsX(static_cast<int>(x));
    SetPosAbsY(static_cast<int>(y));

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
    // Check if text has a valid font before getting bounds
    if (m_text.getFont() == nullptr)
    {
      return; // Cannot position text without a valid font
    }

    sf::FloatRect textBounds = m_text.getLocalBounds();
    float posX = static_cast<float>(GetPosAbsX());
    float posY = static_cast<float>(GetPosAbsY());

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

  void WidgetText::UpdatePosition()
  {
    // Called when position is changed by container - update SFML text position
    ApplicationUpdateTextPosition();
  }

  // Fading functionality implementation
  void WidgetText::SetFadingEnabled(bool enabled)
  {
    m_fadingEnabled = enabled;
  }

  void WidgetText::SetFadingSpeed(float durationSeconds)
  {
    // Convert duration in seconds to alpha change per second
    if (durationSeconds > 0.0f)
    {
      m_fadingSpeed = 255.0f / durationSeconds;
    }
    else
    {
      m_fadingSpeed = 255.0f; // Default 1 second
    }
  }

  void WidgetText::SetFadeDirection(bool fadeIn)
  {
    m_fadeDirection = fadeIn;
  }

  void WidgetText::StartFading(bool fadeIn, float durationSeconds)
  {
    m_fadeDirection = fadeIn;

    // Convert duration in seconds to alpha change per second
    if (durationSeconds > 0.0f)
    {
      m_fadingSpeed = 255.0f / durationSeconds;
    }
    else
    {
      m_fadingSpeed = 255.0f; // Default 1 second
    }

    m_fadingEnabled = true;

    // Set starting alpha based on fade direction
    if (fadeIn)
    {
      m_currentAlpha = 0.0f; // Start transparent for fade in
    }
    else
    {
      m_currentAlpha = 255.0f; // Start opaque for fade out
    }

    UpdateTextAlpha();
  }

  void WidgetText::StopFading()
  {
    m_fadingEnabled = false;
  }

  bool WidgetText::IsFading() const
  {
    return m_fadingEnabled;
  }

  float WidgetText::GetCurrentAlpha() const
  {
    return m_currentAlpha;
  }

  void WidgetText::UpdateTextAlpha()
  {
    // Create new color with updated alpha while preserving RGB
    sf::Color newColor = m_originalColor;
    newColor.a = static_cast<sf::Uint8>(m_currentAlpha);
    m_text.setFillColor(newColor);
  }

  void WidgetText::FadeShow(float durationSeconds)
  {
    StartFading(true, durationSeconds);  // Fade in (show)
  }

  void WidgetText::FadeHide(float durationSeconds)
  {
    StartFading(false, durationSeconds); // Fade out (hide)
  }

}