#pragma once
#include "widget.h"
#include <vector>

namespace ui
{
  enum class LayoutType
  {
    Native,     // Manual positioning - widgets keep their original positions
    Horizontal, // Widgets arranged horizontally side-by-side with optional spacing
    Vertical    // Widgets arranged vertically top-to-bottom with optional spacing
  };

  class WidgetContainer : public Widget
  {
  public:
    WidgetContainer(int posX, int posY, int width, int height);
    virtual ~WidgetContainer();

    // Widget interface implementation
    virtual InputEventState ProcessInput(const InputEvent& event) override;
    virtual void Draw(RenderContext& context) const override;

    // Container specific methods
    void AddWidget(WidgetPtr widget);
    void RemoveWidget(const Widget* widget);
    void ClearWidgets();
    size_t GetWidgetCount() const;

    // Layout management
    void SetLayout(LayoutType layout, int spacing = 0);
    LayoutType GetLayout() const { return m_layoutType; }
    int GetSpacing() const { return m_spacing; }
    void UpdateLayout(); // Recalculate layout after hierarchy changes

    // (Removed local EnableDebugDraw; now inherited from Widget)

  private:
    void ApplyLayout();
    void RecomputeChildrenAbsolutePositions();

    std::vector<WidgetPtr> m_children;
    bool m_debugDraw = false;
    sf::Color m_debugColor;
    LayoutType m_layoutType = LayoutType::Native;
    int m_spacing = 0;
  };
}
