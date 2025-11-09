#pragma once
#include "../framework/WidgetText.h"
#include "../framework/WidgetImage.h"
#include "../framework/WidgetButton.h"
#include "../framework/WidgetContainer.h"
#include "../framework/WidgetProgressBar.h"
#include <SFML/Graphics.hpp>
#include "pch.h"

class Application; // Forward declaration

//==============================================================================
// ApplicationUI Class - Handles all UI initialization and management
//==============================================================================

class ApplicationUI
{
public:
  ApplicationUI();
  ~ApplicationUI() = default;

  // Main initialization functions
  void InitializeUI(Application* app);
  void InitializeContainersUI();

  // Individual UI initialization functions
  void UI_InitializeRootContainer();
  void UI_InitializeMonitorMenuContainer();
  void UI_InitializeTradeContainer();
  void UI_InitializeInventoryContainer();
  void UI_InitializeInfoContainer();
  void UI_InitializeGameTimeWidget();
  void UI_InitializeImageWidgets();
  void UI_InitializeProgressBars();
  void UI_DebugContainers();
  void LoadingNewText(bool first);

  // Update functions
  void UpdateProductDisplays();
  void UpdateApplicationUI( sf::Time delta);
  
  // Monitor selection functions
  void SelectMonitor(int monitorIndex);
  void CancelSelection();

  // Getters for UI components
  std::unique_ptr<ui::WidgetContainer>& GetRootContainer() { return m_rootContainer; }
  ui::WidgetContainer* GetMonitorMenuContainer() const { return m_monitorMenuContainer; }
  ui::WidgetContainer* GetTradeContainer() const { return m_tradeContainer; }
  ui::WidgetContainer* GetInventoryContainer() const { return m_inventoryContainer; }
  ui::WidgetContainer* GetInfoContainer() const { return m_infoContainer; }
  ui::WidgetText* GetGameTimeText() const { return m_gameTimeText; }
  ui::WidgetText* GetRollingText1() const { return m_rollingText1; }
  ui::WidgetText* GetRollingText2() const { return m_rollingText2; }

  // Getters for Image Widgets
  ui::WidgetImage* GetIconMaterialLumi() const { return m_iconMaterialLumi; }
  ui::WidgetImage* GetIconMaterialNano() const { return m_iconMaterialNano; }
  ui::WidgetImage* GetIconMaterialNeuro() const { return m_iconMaterialNeuro; }
  ui::WidgetImage* GetIconMaterialTritanium() const { return m_iconMaterialTritanium; }
  ui::WidgetImage* GetIconMaterialZeromass() const { return m_iconMaterialZeromass; }
  ui::WidgetImage* GetImageTrendArrowDown(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_imageTrendArrowDown[monitorIndex] : nullptr; }
  ui::WidgetImage* GetImageTrendArrowUp(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_imageTrendArrowUp[monitorIndex] : nullptr; }
  ui::WidgetButton* GetMonitorButton(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_monitorButtons[monitorIndex] : nullptr; }
  ui::WidgetImage* GetMonitorHighlight(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_monitorHighlights[monitorIndex] : nullptr; }
  ui::WidgetButton* GetConfirmTradeButton() const { return m_confirmTradeButton; }
  ui::WidgetButton* GetCancelTradeButton() const { return m_cancelTradeButton; }

  // Getters for Text Widgets
  ui::WidgetText* GettxtProd(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_txtProd[monitorIndex] : nullptr; }
  ui::WidgetText* GettxtProdQuantity(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_txtProdQuantity[monitorIndex] : nullptr; }
  ui::WidgetText* GettxtProdPrice(int monitorIndex) const { return (monitorIndex >= 0 && monitorIndex < 5) ? m_txtProdPrice[monitorIndex] : nullptr; }

  // Getters for Progress Bars
  ui::WidgetProgressBar* GetHealthProgressBar() const { return m_healthProgressBar; }
  ui::WidgetProgressBar* GetEnergyProgressBar() const { return m_energyProgressBar; }

  // Getters for Inventory Container Widgets
  ui::WidgetText* GetVolumeText() const { return m_volumeText; }
  ui::WidgetProgressBar* GetVolumeProgressBar() const { return m_volumeProgressBar; }

  // Getters for Info Container Widgets
  ui::WidgetImage* GetCompanyInfoImage() const { return m_companyInfoImage; }
  ui::WidgetText* GetProductInfoText() const { return m_productInfoText; }
  ui::WidgetText* GetProductVolumeText() const { return m_productVolumeText; }

private:
  // Application reference
  Application* m_application = nullptr;

  // UI variables
  std::unique_ptr<ui::WidgetContainer> m_rootContainer; // Root container for all widgets
  ui::WidgetContainer* m_monitorMenuContainer; // Pointer to monitor menu container (owned by root container)
  ui::WidgetContainer* m_tradeContainer; // Pointer to trade container with confirm/cancel buttons
  ui::WidgetContainer* m_inventoryContainer; // Pointer to inventory container (bottom left)
  ui::WidgetContainer* m_infoContainer; // Pointer to info container (bottom right)
  ui::WidgetText* m_gameTimeText; // Pointer to game time text widget (owned by container)
  ui::WidgetText* m_rollingText1; // Rolling text widget 1 at top of screen
  ui::WidgetText* m_rollingText2; // Rolling text widget 2 at top of screen

  // Monitor containers
  ui::WidgetContainer* m_monitor1Container;
  ui::WidgetContainer* m_monitor2Container;
  ui::WidgetContainer* m_monitor3Container;
  ui::WidgetContainer* m_monitor4Container;
  ui::WidgetContainer* m_monitor5Container;
  
  // Monitor buttons for selection management
  ui::WidgetButton* m_monitorButtons[5];     // Array of monitor buttons for selection
  ui::WidgetImage* m_monitorHighlights[5];  // Array of purple highlight overlays for selected monitors
  int m_selectedMonitorIndex;               // Currently selected monitor (-1 if none)

  // Trade container buttons
  ui::WidgetButton* m_confirmTradeButton;   // Confirm trade action button
  ui::WidgetButton* m_cancelTradeButton;    // Cancel trade action button

  // Material Icons
  ui::WidgetImage* m_iconMaterialLumi;
  ui::WidgetImage* m_iconMaterialNano;
  ui::WidgetImage* m_iconMaterialNeuro;
  ui::WidgetImage* m_iconMaterialTritanium;
  ui::WidgetImage* m_iconMaterialZeromass;

  // Trend Images - Arrays for multiple monitors
  ui::WidgetImage* m_imageTrendArrowDown[5]; // One for each monitor
  ui::WidgetImage* m_imageTrendArrowUp[5];   // One for each monitor

  // Monitor Text Widgets - Arrays for multiple monitors
  ui::WidgetText* m_txtProd[5];       // Product monitor text for each monitor
  ui::WidgetText* m_txtProdQuantity[5];     // Current quantity text for each monitor
  ui::WidgetText* m_txtProdPrice[5];         // Trend text for each monitor

  // Progress Bars
  ui::WidgetProgressBar* m_healthProgressBar;  // Health/HP progress bar
  ui::WidgetProgressBar* m_energyProgressBar;  // Energy/Mana progress bar

  // Inventory container widgets
  ui::WidgetText* m_volumeText;              // Volume text in inventory container
  ui::WidgetProgressBar* m_volumeProgressBar; // Volume progress bar in inventory container

  // Info container widgets
  ui::WidgetImage* m_companyInfoImage;       // Company info image in info container
  ui::WidgetText* m_productInfoText;         // Product info text in info container
  ui::WidgetText* m_productVolumeText;       // Product volume text in info container

  // Fonts
  sf::Font m_digitalFont; // Font for game time display
  sf::Font m_ledFont; // Font for rolling news ticker

  // Rolling text animation variables
  float m_rollingText1Position; // Current X position of rolling text 1
  float m_rollingText2Position; // Current X position of rolling text 2
  float m_rollingSpeed; // Speed of rolling animation (pixels per second)
};