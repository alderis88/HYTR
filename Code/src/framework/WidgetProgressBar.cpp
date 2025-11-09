#include "pch.h"
#include "WidgetProgressBar.h"
#include <sstream>
#include <iomanip>

namespace ui
{
    WidgetProgressBar::WidgetProgressBar(int posX, int posY, int width, int height, const std::string& suffix)
        : Widget(posX, posY, width, height)
        , m_progress(0.0f)
        , m_backgroundColor(sf::Color(64, 64, 64))      // Dark gray
        , m_foregroundColor(sf::Color(0, 150, 255))     // Blue
        , m_borderColor(sf::Color::Black)
        , m_textColor(sf::Color::White)
        , m_borderThickness(2.0f)
        , m_borderEnabled(true)
        , m_showPercentage(true)
        , m_showText(true)
        , m_suffix(suffix)
        , m_customText("")
        , m_textSize(14)
    {
        // Initialize shapes
        UpdateShapes();
    }

    WidgetProgressBar::~WidgetProgressBar() = default;

    InputEventState WidgetProgressBar::ProcessInput(const InputEvent& event)
    {
        // Progress bar doesn't handle input by default
        return InputEventState::Unhandled;
    }

    void WidgetProgressBar::Draw(RenderContext& context) const
    {
        if (!IsVisible())
            return;

        UpdateShapes();

        // Draw background
        context.draw(m_backgroundRect);

        // Draw progress foreground
        if (m_progress > 0.0f)
        {
            context.draw(m_foregroundRect);
        }

        // Draw border
        if (m_borderEnabled && m_borderThickness > 0.0f)
        {
            context.draw(m_borderRect);
        }

        // Draw text if enabled
        if (m_showText)
        {
            context.draw(m_percentageText);
        }
    }

    void WidgetProgressBar::SetProgress(float progress)
    {
        // Clamp progress between 0.0 and 1.0
        m_progress = std::max(0.0f, std::min(1.0f, progress));
    }

    float WidgetProgressBar::GetProgress() const
    {
        return m_progress;
    }

    void WidgetProgressBar::SetBackgroundColor(const sf::Color& color)
    {
        m_backgroundColor = color;
    }

    void WidgetProgressBar::SetForegroundColor(const sf::Color& color)
    {
        m_foregroundColor = color;
    }

    void WidgetProgressBar::SetBorderColor(const sf::Color& color)
    {
        m_borderColor = color;
    }

    void WidgetProgressBar::SetBorderThickness(float thickness)
    {
        m_borderThickness = thickness;
    }

    void WidgetProgressBar::SetBorderEnabled(bool enabled)
    {
        m_borderEnabled = enabled;
    }

    void WidgetProgressBar::SetShowPercentage(bool show)
    {
        m_showPercentage = show;
    }

    void WidgetProgressBar::SetFont(const sf::Font& font)
    {
        m_font = font;
        m_percentageText.setFont(m_font);
    }

    void WidgetProgressBar::SetTextColor(const sf::Color& color)
    {
        m_textColor = color;
    }

    void WidgetProgressBar::SetTextSize(unsigned int size)
    {
        m_textSize = size;
    }

    void WidgetProgressBar::SetSuffix(const std::string& suffix)
    {
        m_suffix = suffix;
    }

    void WidgetProgressBar::SetShowText(bool show)
    {
        m_showText = show;
    }

    void WidgetProgressBar::SetCustomText(const std::string& text)
    {
        m_customText = text;
    }

    void WidgetProgressBar::UpdateShapes() const
    {
        float posX = static_cast<float>(GetPosAbsX());
        float posY = static_cast<float>(GetPosAbsY());
        float width = static_cast<float>(GetWidth());
        float height = static_cast<float>(GetHeight());

        // Update background rectangle
        m_backgroundRect.setPosition(posX, posY);
        m_backgroundRect.setSize(sf::Vector2f(width, height));
        m_backgroundRect.setFillColor(m_backgroundColor);

        // Update foreground rectangle (progress fill)
        float progressWidth = width * m_progress;
        m_foregroundRect.setPosition(posX, posY);
        m_foregroundRect.setSize(sf::Vector2f(progressWidth, height));
        m_foregroundRect.setFillColor(m_foregroundColor);

        // Update border rectangle
        m_borderRect.setPosition(posX - m_borderThickness, posY - m_borderThickness);
        m_borderRect.setSize(sf::Vector2f(width + 2 * m_borderThickness, height + 2 * m_borderThickness));
        m_borderRect.setFillColor(sf::Color::Transparent);
        m_borderRect.setOutlineThickness(m_borderThickness);
        m_borderRect.setOutlineColor(m_borderColor);

        // Update text (percentage or custom)
        if (m_showText)
        {
            std::string displayText;

            if (!m_customText.empty())
            {
                // Use custom text if provided
                displayText = m_customText;
            }
            else if (m_showPercentage)
            {
                // Generate percentage text with suffix
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(0) << (m_progress * 100.0f) << m_suffix;
                displayText = oss.str();
            }

            m_percentageText.setString(displayText);
            m_percentageText.setFillColor(m_textColor);
            m_percentageText.setCharacterSize(m_textSize); // Set smaller text size

            // Set font if available
            if (m_font.getInfo().family != "")
            {
                m_percentageText.setFont(m_font);
            }

            // Calculate text size for centering
            sf::FloatRect textBounds = m_percentageText.getLocalBounds();
            float textX = posX + (width - textBounds.width) / 2.0f;
            float textY = posY + (height - textBounds.height) / 2.0f - textBounds.top;
            m_percentageText.setPosition(textX, textY);
        }
    }
}