#pragma once
#include "widget.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>

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
    virtual void Update(float deltaTime);

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

    // Fading functionality
    void SetFadingEnabled(bool enabled);
    void SetFadingSpeed(float durationSeconds); // Duration in seconds for full fade (0->255 or 255->0)
    void SetFadeDirection(bool fadeIn); // true = fade in, false = fade out
    void StartFading(bool fadeIn, float durationSeconds = 1.0f);
    void StopFading();
    bool IsFading() const;
    float GetCurrentAlpha() const;

    // Convenience methods
    void FadeShow(float durationSeconds); // Fade in (show) over duration
    void FadeHide(float durationSeconds); // Fade out (hide) over duration

    // Override position update notification
    void UpdatePosition() override;

  private:
    sf::Text m_text;
    sf::Font m_defaultFont;
    bool m_hasCustomFont;
    Alignment m_alignment;
    std::string m_textString;

    // Fading variables
    bool m_fadingEnabled;
    bool m_fadeDirection; // true = fade in, false = fade out
    float m_fadingSpeed; // Alpha change per second
    float m_currentAlpha; // Current alpha value (0-255)
    sf::Color m_originalColor; // Store original color for fading

    void ApplicationUpdateTextPosition();
    void UpdateTextAlpha();
  };

  using WidgetTextPtr = std::unique_ptr<WidgetText>;
}