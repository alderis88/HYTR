#pragma once
#include "widget.h"
#include <SFML/Graphics.hpp>

namespace ui
{
    class WidgetProgressBar : public Widget
    {
    public:
        WidgetProgressBar(int posX, int posY, int width, int height, const std::string& suffix = "%");
        virtual ~WidgetProgressBar();

        // Widget interface implementation
        virtual InputEventState ProcessInput(const InputEvent& event) override;
        virtual void Draw(RenderContext& context) const override;

        // Progress bar specific methods
        void SetProgress(float progress); // 0.0f - 1.0f
        float GetProgress() const;

        void SetBackgroundColor(const sf::Color& color);
        void SetForegroundColor(const sf::Color& color);
        void SetBorderColor(const sf::Color& color);
        void SetBorderThickness(float thickness);

        // Text display (optional)
        void SetShowPercentage(bool show);
        void SetFont(const sf::Font& font);
        void SetTextColor(const sf::Color& color);
        void SetTextSize(unsigned int size);
        void SetSuffix(const std::string& suffix);
        void SetShowText(bool show);
        void SetCustomText(const std::string& text); // Custom text instead of percentage

    private:
        float m_progress;

        sf::Color m_backgroundColor;
        sf::Color m_foregroundColor;
        sf::Color m_borderColor;
        sf::Color m_textColor;

        float m_borderThickness;
        bool m_showPercentage;
        bool m_showText;
        std::string m_suffix;
        std::string m_customText;
        unsigned int m_textSize;

        sf::Font m_font;

        // SFML shapes for rendering
        mutable sf::RectangleShape m_backgroundRect;
        mutable sf::RectangleShape m_foregroundRect;
        mutable sf::RectangleShape m_borderRect;
        mutable sf::Text m_percentageText;

        void UpdateShapes() const;
    };
}