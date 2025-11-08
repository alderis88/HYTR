#pragma once
#include "../framework/WidgetText.h"
#include "../framework/WidgetImage.h"
#include "../framework/WidgetButton.h"
#include "../framework/WidgetContainer.h"
#include <SFML/Graphics.hpp>
#include "pch.h"

//==============================================================================
// ApplicationUI Class - Handles all UI initialization and management
//==============================================================================

class ApplicationUI
{
public:
  ApplicationUI();
  ~ApplicationUI() = default;

  // Main initialization functions
  void InitializeUI();
  void InitializeContainersUI();

  // Individual UI initialization functions
  void UI_InitializeRootContainer();
  void UI_InitializeMonitorMenuContainer();
  void UI_InitializeSubMenuContainer();
  void UI_InitializeGameTimeWidget();
  void UI_InitializeImageWidgets();
  void UI_DebugContainers();

  // Getters for UI components
  std::unique_ptr<ui::WidgetContainer>& GetRootContainer() { return m_rootContainer; }
  ui::WidgetContainer* GetMonitorMenuContainer() const { return m_monitorMenuContainer; }
  ui::WidgetContainer* GetSubMenuContainer() const { return m_subMenuContainer; }
  ui::WidgetText* GetGameTimeText() const { return m_gameTimeText; }

  // Getters for Image Widgets
  ui::WidgetImage* GetIconMaterialLumi() const { return m_iconMaterialLumi; }
  ui::WidgetImage* GetIconMaterialNano() const { return m_iconMaterialNano; }
  ui::WidgetImage* GetIconMaterialNeuro() const { return m_iconMaterialNeuro; }
  ui::WidgetImage* GetIconMaterialTritanium() const { return m_iconMaterialTritanium; }
  ui::WidgetImage* GetIconMaterialZeromass() const { return m_iconMaterialZeromass; }
  ui::WidgetImage* GetImageTrendArrowDown() const { return m_imageTrendArrowDown; }
  ui::WidgetImage* GetImageTrendArrowUp() const { return m_imageTrendArrowUp; }

private:
  // UI variables
  std::unique_ptr<ui::WidgetContainer> m_rootContainer; // Root container for all widgets
  ui::WidgetContainer* m_monitorMenuContainer; // Pointer to monitor menu container (owned by root container)
  ui::WidgetContainer* m_subMenuContainer;  // Pointer to sub menu container (owned by root container)
  ui::WidgetText* m_gameTimeText; // Pointer to game time text widget (owned by container)

  // Monitor containers
  ui::WidgetContainer* m_monitor1Container;
  ui::WidgetContainer* m_monitor2Container;
  ui::WidgetContainer* m_monitor3Container;
  ui::WidgetContainer* m_monitor4Container;
  ui::WidgetContainer* m_monitor5Container;

  // Material Icons
  ui::WidgetImage* m_iconMaterialLumi;
  ui::WidgetImage* m_iconMaterialNano;
  ui::WidgetImage* m_iconMaterialNeuro;
  ui::WidgetImage* m_iconMaterialTritanium;
  ui::WidgetImage* m_iconMaterialZeromass;

  // Trend Images
  ui::WidgetImage* m_imageTrendArrowDown;
  ui::WidgetImage* m_imageTrendArrowUp;

  // Fonts
  sf::Font m_digitalFont; // Font for game time display
};