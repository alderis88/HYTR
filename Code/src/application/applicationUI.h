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
// Enums for UI functionality
//==============================================================================

/// @brief Inventory sort types for sorting inventory items
enum class InventorySortType
{
  Volume,   // Sort by item volume
  Quantity  // Sort by item quantity
};

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
  void UI_InitializeProductInfoContainer();
  void UI_InitializeCompanyInfoContainer();
  void UI_InitializeVendorInfoContainer();
  void UI_InitializeInfoPanelSelector();
  void UI_InitializeInventorySortSelector();
  void UI_InitializeGameTimeWidget();
  void UI_InitializeImageWidgets();
  void UI_InitializeProgressBars();
  void UI_InitializeLogo();
  void UI_DebugContainers();
  void LoadingNewText(bool first);

  // Update functions
  void UpdateProductDisplays();
  void UpdateCycleProgressBar();
  void UpdateCurrentMoneyDisplay();
  void UpdateInventoryButtons();
  void UpdateApplicationUI( sf::Time delta);
  
  // Monitor selection functions
  void SelectMonitor(int monitorIndex);
  void CancelSelection();

  // Info panel selection functions
  void SelectInfoPanel(int panelIndex);

  // Inventory sort selection functions
  void SelectInventorySort(InventorySortType sortType);

  // Getters for UI components
  std::unique_ptr<ui::WidgetContainer>& GetRootContainer() { return m_rootContainer; }
  ui::WidgetContainer* GetMonitorMenuContainer() const { return m_monitorMenuContainer; }
  ui::WidgetContainer* GetTradeContainer() const { return m_tradeContainer; }
  ui::WidgetContainer* GetInventoryContainer() const { return m_inventoryContainer; }
  ui::WidgetContainer* GetProductInfoContainer() const { return m_productInfoContainer; }
  ui::WidgetContainer* GetCompanyInfoContainer() const { return m_companyInfoContainer; }
  ui::WidgetContainer* GetVendorInfoContainer() const { return m_vendorInfoContainer; }
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
  ui::WidgetProgressBar* GetCycleProgressBar() const { return m_cycleProgressBar; }
  ui::WidgetProgressBar* GetEnergyProgressBar() const { return m_energyProgressBar; }

  // Getters for Inventory Container Widgets
  ui::WidgetText* GetVolumeText() const { return m_volumeText; }
  ui::WidgetProgressBar* GetVolumeProgressBar() const { return m_volumeProgressBar; }

  // Getters for Product Info Container Widgets
  ui::WidgetImage* GetProductInfoImage() const { return m_productInfoImage; }
  ui::WidgetText* GetProductInfoText() const { return m_productInfoText; }
  ui::WidgetText* GetProductVolumeText() const { return m_productVolumeText; }
  ui::WidgetText* GetProductRarityText() const { return m_productRarityText; }

  // Getters for Company Info Container Widgets
  ui::WidgetText* GetCompanyNameText() const { return m_companyNameText; }
  ui::WidgetText* GetVendorRoleText() const { return m_vendorRoleText; }
  ui::WidgetText* GetProductNameText() const { return m_productNameText; }

private:
  // Application reference
  Application* m_application = nullptr;

  // UI variables
  std::unique_ptr<ui::WidgetContainer> m_rootContainer; // Root container for all widgets
  ui::WidgetContainer* m_monitorMenuContainer; // Pointer to monitor menu container (owned by root container)
  ui::WidgetContainer* m_tradeContainer; // Pointer to trade container with confirm/cancel buttons
  ui::WidgetContainer* m_inventoryContainer; // Pointer to inventory container (bottom left)
  ui::WidgetContainer* m_productInfoContainer; // Pointer to product info container (bottom right)
  ui::WidgetContainer* m_companyInfoContainer; // Pointer to company info container (bottom right)
  ui::WidgetContainer* m_vendorInfoContainer; // Pointer to vendor info container (bottom right)
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

  // Trade container buttons and text
  ui::WidgetButton* m_confirmTradeButton;   // Confirm trade action button
  ui::WidgetButton* m_cancelTradeButton;    // Cancel trade action button
  ui::WidgetText* m_currentMoneyText;       // Current money display text

  // Info panel selector container and buttons
  ui::WidgetContainer* m_infoPanelSelectorContainer; // Container for info panel selector buttons
  ui::WidgetButton* m_productInfoSelectorButton;     // Product info selector button
  ui::WidgetButton* m_companyInfoSelectorButton;     // Company info selector button  
  ui::WidgetButton* m_vendorInfoSelectorButton;      // Vendor info selector button
  int m_selectedInfoPanel;                           // Currently selected info panel (0=Product, 1=Company, 2=Vendor)

  // Inventory sort selector container and buttons
  ui::WidgetContainer* m_inventorySortSelectorContainer; // Container for inventory sort selector buttons
  ui::WidgetButton* m_volumeSortButton;                  // Volume sort selector button
  ui::WidgetButton* m_quantitySortButton;                // Quantity sort selector button
  InventorySortType m_selectedSortType;                  // Currently selected sort type

  // Material Icons
  ui::WidgetImage* m_iconMaterialLumi;
  ui::WidgetImage* m_iconMaterialNano;
  ui::WidgetImage* m_iconMaterialNeuro;
  ui::WidgetImage* m_iconMaterialTritanium;
  ui::WidgetImage* m_iconMaterialZeromass;

  // Logo
  ui::WidgetImage* m_logoImage;

  // Trend Images - Arrays for multiple monitors
  ui::WidgetImage* m_imageTrendArrowDown[5]; // One for each monitor
  ui::WidgetImage* m_imageTrendArrowUp[5];   // One for each monitor

  // Monitor Text Widgets - Arrays for multiple monitors
  ui::WidgetText* m_txtProd[5];       // Product monitor text for each monitor
  ui::WidgetText* m_txtProdQuantity[5];     // Current quantity text for each monitor
  ui::WidgetText* m_txtProdPrice[5];         // Trend text for each monitor

  // Progress Bars
  ui::WidgetProgressBar* m_cycleProgressBar;   // Cycle progress bar (top center)
  ui::WidgetProgressBar* m_energyProgressBar;  // Energy/Mana progress bar

  // Inventory container widgets
  ui::WidgetText* m_volumeText;              // Volume text in inventory container
  ui::WidgetProgressBar* m_volumeProgressBar; // Volume progress bar in inventory container
  
  // Inventory product containers and buttons
  ui::WidgetContainer* m_inventoryProductsContainer;     // Vertical container for inventory products
  ui::WidgetContainer* m_triInvContainer;                // Container for Tritanium inventory button
  ui::WidgetContainer* m_nfxInvContainer;                // Container for Neuroflux inventory button
  ui::WidgetContainer* m_zerInvContainer;                // Container for Zeromass inventory button
  ui::WidgetContainer* m_lumInvContainer;                // Container for Lumirite inventory button
  ui::WidgetContainer* m_nanInvContainer;                // Container for Nanochip inventory button
  ui::WidgetButton* m_triInvButton;                      // Tritanium inventory button
  ui::WidgetButton* m_nfxInvButton;                      // Neuroflux inventory button
  ui::WidgetButton* m_zerInvButton;                      // Zeromass inventory button
  ui::WidgetButton* m_lumInvButton;                      // Lumirite inventory button
  ui::WidgetButton* m_nanInvButton;                      // Nanochip inventory button
  ui::WidgetImage* m_triInvIcon;                         // Tritanium inventory icon
  ui::WidgetImage* m_nfxInvIcon;                         // Neuroflux inventory icon
  ui::WidgetImage* m_zerInvIcon;                         // Zeromass inventory icon
  ui::WidgetImage* m_lumInvIcon;                         // Lumirite inventory icon
  ui::WidgetImage* m_nanInvIcon;                         // Nanochip inventory icon
  ui::WidgetText* m_triInvQuantityText;                  // Tritanium inventory quantity text
  ui::WidgetText* m_nfxInvQuantityText;                  // Neuroflux inventory quantity text
  ui::WidgetText* m_zerInvQuantityText;                  // Zeromass inventory quantity text
  ui::WidgetText* m_lumInvQuantityText;                  // Lumirite inventory quantity text
  ui::WidgetText* m_nanInvQuantityText;                  // Nanochip inventory quantity text
  ui::WidgetText* m_triInvVolumeText;                    // Tritanium inventory volume text
  ui::WidgetText* m_nfxInvVolumeText;                    // Neuroflux inventory volume text
  ui::WidgetText* m_zerInvVolumeText;                    // Zeromass inventory volume text
  ui::WidgetText* m_lumInvVolumeText;                    // Lumirite inventory volume text
  ui::WidgetText* m_nanInvVolumeText;                    // Nanochip inventory volume text

  // Product Info container widgets
  ui::WidgetImage* m_productInfoImage;       // Product info image in product info container
  ui::WidgetText* m_productInfoText;         // Product info text in product info container
  ui::WidgetText* m_productVolumeText;       // Product volume text in product info container
  ui::WidgetText* m_productRarityText;       // Product rarity text in product info container
  ui::WidgetText* m_productNameText;         // Product name text in product info container

  // Company Info container widgets
  ui::WidgetText* m_companyNameText;         // Company name text in company info container
  ui::WidgetText* m_companyInfoText;         // Company detailed info text in company info container
  ui::WidgetImage* m_companyLogo;            // Company logo image in company info container
 
  // Vendor info container widgets
  ui::WidgetText* m_vendorInfoText;           // Vendor info text in vendor info container
  ui::WidgetText* m_vendorNameText;           // Vendor name text in vendor info container
  ui::WidgetText* m_vendorProfileText;        // Vendor profile text in vendor info container
   ui::WidgetImage* m_vendorImage;            // Vendor character image in vendor info container
  ui::WidgetText* m_vendorRoleText;           // Vendor role text in vendor info container

  // Fonts
  sf::Font m_digitalFont; // Font for game time display
  sf::Font m_ledFont; // Font for rolling news ticker

  // Rolling text animation variables
  float m_rollingText1Position; // Current X position of rolling text 1
  float m_rollingText2Position; // Current X position of rolling text 2
  float m_rollingSpeed; // Speed of rolling animation (pixels per second)
};