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
    // Only process input if this container is visible
    if (!IsVisible())
      return InputEventState::Unhandled;

    // Process input for all visible children, starting from the last added (top-most)
    // This allows for proper event handling in cases of overlapping widgets
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
    {
      if ((*it)->IsVisible() && (*it)->ProcessInput(event) == InputEventState::Handled)
      {
        return InputEventState::Handled;
      }
    }
    return InputEventState::Unhandled;
  }

  void WidgetContainer::Draw(RenderContext& context) const
  {
    // Only draw if this container is visible
    if (!IsVisible())
      return;

    // Draw all visible children in order (first added = bottom-most)
    for (const auto& child : m_children)
    {
      if (child->IsVisible())
      {
        child->Draw(context);
      }
    }

    // Draw debug bounds if enabled (after children so it's always visible on top)
    if (m_debugDraw)
    {
      sf::RectangleShape debugRect(sf::Vector2f(static_cast<float>(GetWidth()-2), static_cast<float>(GetHeight()-2)));
      debugRect.setPosition(static_cast<float>(GetPosAbsX()+1), static_cast<float>(GetPosAbsY())+1);
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
      // Store the original position as relative position
      int relX = widget->GetPosRelX();  // Widget's initial position is stored as relative
      int relY = widget->GetPosRelY();
      widget->SetPosRelX(relX);
      widget->SetPosRelY(relY);

      // Convert relative position to absolute position based on container position
      if (m_layoutType == LayoutType::Native)
      {
        // Calculate absolute position by adding container's absolute position
        int absX = relX + GetPosAbsX();
        int absY = relY + GetPosAbsY();

        // Position calculation completed successfully

        widget->SetPosAbsX(absX);
        widget->SetPosAbsY(absY);

        // Notify widget that position has changed (important for text widgets!)
        widget->UpdatePosition();
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

    int currentX = GetPosAbsX();
    int currentY = GetPosAbsY();

    for (auto& child : m_children)
    {
      if (m_layoutType == LayoutType::Horizontal)
      {
        // Horizontal: Control X position, preserve relative Y
        child->SetPosAbsX(currentX);
        child->SetPosAbsY(GetPosAbsY() + child->GetPosRelY());
        child->UpdatePosition();
        currentX += child->GetWidth() + m_spacing;
      }
      else if (m_layoutType == LayoutType::Vertical)
      {
        // Vertical: Control Y position, preserve relative X
        child->SetPosAbsX(GetPosAbsX() + child->GetPosRelX());
        child->SetPosAbsY(currentY);
        child->UpdatePosition();
        currentY += child->GetHeight() + m_spacing;
      }
    }
  }
}
