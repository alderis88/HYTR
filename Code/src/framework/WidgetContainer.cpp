#include "pch.h"
#include "WidgetContainer.h"

namespace ui
{
  WidgetContainer::WidgetContainer(int posX, int posY, int width, int height)
    : Widget(posX, posY, width, height)
  {
  }

  WidgetContainer::~WidgetContainer()
  {
    // std::unique_ptr will automatically clean up children
  }

  InputEventState WidgetContainer::ProcessInput(const InputEvent& event)
  {
    // Process input for all children, starting from the last added (top-most)
    // This allows for proper event handling in cases of overlapping widgets
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
    {
      if ((*it)->ProcessInput(event) == InputEventState::Handled)
      {
        return InputEventState::Handled;
      }
    }
    return InputEventState::Unhandled;
  }

  void WidgetContainer::Draw(RenderContext& context) const
  {
    // Draw all children in order (first added = bottom-most)
    for (const auto& child : m_children)
    {
      child->Draw(context);
    }

    // Draw debug bounds if enabled (after children so it's always visible on top)
    if (m_debugDraw)
    {
      sf::RectangleShape debugRect(sf::Vector2f(static_cast<float>(GetWidth()), static_cast<float>(GetHeight())));
      debugRect.setPosition(static_cast<float>(GetPosX()), static_cast<float>(GetPosY()));
      debugRect.setFillColor(sf::Color::Transparent);
      debugRect.setOutlineColor(m_debugColor);
      debugRect.setOutlineThickness(1.0f);
      context.draw(debugRect);
    }
  }

  void WidgetContainer::AddWidget(WidgetPtr widget)
  {
    if (widget)
    {
      // Convert relative position to absolute position based on container position
      if (m_layoutType == LayoutType::Native)
      {
        widget->SetPosX(widget->GetPosX() + GetPosX());
        widget->SetPosY(widget->GetPosY() + GetPosY());
      }

      m_children.push_back(std::move(widget));
      ApplyLayout();
    }
  }

  void WidgetContainer::RemoveWidget(const Widget* widget)
  {
    if (!widget) return;

    auto it = std::find_if(m_children.begin(), m_children.end(),
      [widget](const WidgetPtr& ptr) { return ptr.get() == widget; });

    if (it != m_children.end())
    {
      m_children.erase(it);
      ApplyLayout();
    }
  }

  void WidgetContainer::ClearWidgets()
  {
    m_children.clear();
  }

  size_t WidgetContainer::GetWidgetCount() const
  {
    return m_children.size();
  }

  void WidgetContainer::EnableDebugDraw(bool enable, sf::Color color)
  {
    m_debugDraw = enable;
    m_debugColor = color;
  }

  void WidgetContainer::SetLayout(LayoutType layout, int spacing)
  {
    m_layoutType = layout;
    m_spacing = spacing;
    ApplyLayout();
  }

  void WidgetContainer::UpdateLayout()
  {
    ApplyLayout();
  }

  void WidgetContainer::ApplyLayout()
  {
    if (m_layoutType == LayoutType::Native || m_children.empty())
    {
      return; // No layout adjustment needed
    }

    int currentX = GetPosX();
    int currentY = GetPosY();

    for (auto& child : m_children)
    {
      if (m_layoutType == LayoutType::Horizontal)
      {
        child->SetPosX(currentX);
        child->SetPosY(GetPosY());
        currentX += child->GetWidth() + m_spacing;
      }
      else if (m_layoutType == LayoutType::Vertical)
      {
        child->SetPosX(GetPosX());
        child->SetPosY(currentY);
        currentY += child->GetHeight() + m_spacing;
      }
    }
  }
}
