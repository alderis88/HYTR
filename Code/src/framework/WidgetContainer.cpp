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
    for (const WidgetPtr& child : m_children)
    {
      child->Draw(context);
    }
    // Draw own debug bounds
    DrawDebugBounds(context);
  }

  void WidgetContainer::AddWidget(WidgetPtr widget)
  {
    if (!widget) return;
    widget->SetParent(this); // Computes child's absolute position immediately
    m_children.push_back(std::move(widget));
  }

  void WidgetContainer::RemoveWidget(const Widget* widget)
  {
    if (!widget) return;
    auto it = std::find_if(m_children.begin(), m_children.end(),
      [widget](const WidgetPtr& child) { return child.get() == widget; });
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

  size_t WidgetContainer::GetWidgetCount() const { return m_children.size(); }

  // EnableDebugDraw now inherited

  void WidgetContainer::SetLayout(LayoutType layout, int spacing)
  {
    m_layoutType = layout;
    m_spacing = spacing;
    // NOTE: Don't call ApplyLayout here - it will be called by UpdateLayout()
    // after the hierarchy is complete
  }

  void WidgetContainer::UpdateLayout()
  {
    ApplyLayout();
    // Recursively update child containers
    for (WidgetPtr& child : m_children)
    {
      WidgetContainer* childContainer = dynamic_cast<WidgetContainer*>(child.get());
      if (childContainer)
      {
        childContainer->UpdateLayout();
      }
    }
  }

  void WidgetContainer::ApplyLayout()
  {
    if (m_children.empty()) return;

    if (m_layoutType == LayoutType::Native)
    {
      // Native: children keep their relative positions as provided
      RecomputeChildrenAbsolutePositions();
    }
    else if (m_layoutType == LayoutType::Horizontal)
    {
      int currentX = 0;
      for (WidgetPtr& child : m_children)
      {
        child->SetPosX(currentX);
        child->SetPosY(0);
        currentX += child->GetWidth() + m_spacing;
      }
      RecomputeChildrenAbsolutePositions();
    }
    else if (m_layoutType == LayoutType::Vertical)
    {
      int currentY = 0;
      for (WidgetPtr& child : m_children)
      {
        child->SetPosX(0);
        child->SetPosY(currentY);
        currentY += child->GetHeight() + m_spacing;
      }
      RecomputeChildrenAbsolutePositions();
    }
  }

  void WidgetContainer::RecomputeChildrenAbsolutePositions()
  {
    for (WidgetPtr& child : m_children)
    {
      child->RecomputeAbsolutePosition();
    }
  }
}
