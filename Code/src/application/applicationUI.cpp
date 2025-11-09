/**
 * @file applicationUI.cpp
 * @brief Implementation of the ApplicationUI class for managing the main game UI
 * @details Handles creation and management of all UI widgets including trading monitors,
 *          progress bars, game time display, and product information displays.
 */

#include "pch.h"
#include "application.h" // Aggregates required widget headers
#include "applicationUI.h"
#include "stockMarket.h" // Required for SetCurrentProductID method
#include "inventory.h" // Required for StockProduct structure

/// @brief Default constructor for ApplicationUI
/// @details Initializes all member pointers to nullptr and sets up widget arrays for safe access
ApplicationUI::ApplicationUI()
  : m_monitorMenuContainer(nullptr)
  , m_subMenuContainer(nullptr)
  , m_gameTimeText(nullptr)
  , m_rollingText1(nullptr)
  , m_rollingText2(nullptr)
  , m_monitor1Container(nullptr)
  , m_monitor2Container(nullptr)
  , m_monitor3Container(nullptr)
  , m_monitor4Container(nullptr)
  , m_monitor5Container(nullptr)
  , m_iconMaterialLumi(nullptr)
  , m_iconMaterialNano(nullptr)
  , m_iconMaterialNeuro(nullptr)
  , m_iconMaterialTritanium(nullptr)
  , m_iconMaterialZeromass(nullptr)
  , m_healthProgressBar(nullptr)
  , m_energyProgressBar(nullptr)
{
  // Initialize all widget arrays to nullptr for safe access checks later
  for (int i = 0; i < 5; ++i)
  {
    m_imageTrendArrowDown[i] = nullptr;
    m_imageTrendArrowUp[i] = nullptr;
    m_txtProd[i] = nullptr;
    m_txtProdQuantity[i] = nullptr;
    m_txtProdPrice[i] = nullptr;
  }
}

/// @brief Main UI initialization function - coordinates all container and widget creation
/// @param app Pointer to the main application instance for accessing subsystems
/// @details This function orchestrates the complete UI setup process including:
///          - Storing application reference for later use
///          - Creating all UI containers and widgets through InitializeContainersUI()
///          - Loading initial product data from stock market
///          - Performing final layout calculations for proper widget positioning
void ApplicationUI::InitializeUI(Application* app)
{
  // Store application reference for accessing stock market and inventory data
  m_application = app;

  // Create all containers and their child widgets in the proper hierarchy
  InitializeContainersUI();

  // Load and display initial stock market data on all product monitors
  UpdateProductDisplays();

  // Perform final layout calculations now that all widgets are created and positioned
  m_rootContainer->UpdateLayout();
}

/// @brief Creates all UI containers and calls their individual initialization functions
/// @details This function manages the creation order of UI components to ensure proper hierarchy:
///          1. Root container (full screen background)
///          2. Monitor menu container (5 trading monitors)
///          3. Sub menu container (additional buttons)
///          4. Game time widget (digital clock display)
///          5. Image widgets (material icons and trend arrows)
///          6. Progress bars (health and energy)
///          7. Debug visualization (development aid)
void ApplicationUI::InitializeContainersUI()
{
  // Create the main UI hierarchy in proper dependency order
  UI_InitializeRootContainer();          // Full-screen root container with background
  UI_InitializeMonitorMenuContainer();   // 5 trading monitor displays with click handlers
  UI_InitializeSubMenuContainer();       // Additional menu buttons (expandable)
  UI_InitializeGameTimeWidget();         // Digital time display with custom font
  UI_InitializeImageWidgets();           // Material icons and trend arrows for each monitor
  UI_InitializeProgressBars();           // Player health and energy indicators

  // Enable visual debugging aids for development (container borders)
  UI_DebugContainers();
}

/// @brief Initialize the root container that serves as the base for all UI elements
/// @details Creates a full-screen container (1920x1080) with background image and title text.
///          This container acts as the parent for all other UI widgets and handles the main
///          visual hierarchy. Z-order: background image (bottom) -> title text -> other containers
void ApplicationUI::UI_InitializeRootContainer()
{
  // Create full-screen root container to hold all UI elements
  m_rootContainer = std::make_unique<ui::WidgetContainer>(0, 0, 1920, 1080);

  // Add background image that fills the entire screen (rendered first, at bottom Z-order)
  auto imageWidget = std::make_unique<ui::WidgetImage>(0, 0, 1920, 1080, "BgInit.png");
  m_rootContainer->AddWidget(std::move(imageWidget));

  // Add main title text centered at the top of screen (above background, below other UI)
  auto titleText = std::make_unique<ui::WidgetText>(960, 100, "Hyper Trade");
  titleText->SetCharacterSize(30);           // Large, prominent title
  titleText->SetStyle(sf::Text::Bold);       // Bold for emphasis
  titleText->SetAlignment(ui::WidgetText::Alignment::Center);  // Center-aligned at X position
  titleText->SetTextColor(sf::Color::White); // White text for visibility on background
  m_rootContainer->AddWidget(std::move(titleText));


  // led font for rolling news ticker
  std::string fontPath = Application::s_assetsPath + "FontLed.ttf";
  sf::Font ledFont;
  ledFont.loadFromFile(fontPath);

  // Add rolling text 1 - full width at very top of screen
  auto rollingText1 = std::make_unique<ui::WidgetText>(0, 10, "Tritanium shares plunge 3% amid factory fire");
  rollingText1->SetCharacterSize(30);
  rollingText1->SetFont(ledFont); // Use led font for rolling text 1
  rollingText1->SetStyle(sf::Text::Regular);
  rollingText1->SetAlignment(ui::WidgetText::Alignment::Left);
  rollingText1->SetTextColor(sf::Color::Red);
  rollingText1->SetWidth(1920);  // Full screen width
  rollingText1->SetHeight(40);
  m_rollingText1 = rollingText1.get();
  m_rootContainer->AddWidget(std::move(rollingText1));

  // Add rolling text 2 - full width below the first one
  auto rollingText2 = std::make_unique<ui::WidgetText>(1920,10, "Corporate announcements: Zeromass Labs reports quantum breakthrough...");
  rollingText2->SetCharacterSize(30);
  rollingText2->SetFont(ledFont); // Use led font for rolling text 2
  rollingText2->SetStyle(sf::Text::Regular);
  rollingText2->SetAlignment(ui::WidgetText::Alignment::Left);
  rollingText2->SetTextColor(sf::Color::Cyan);
  rollingText2->SetWidth(1920);  // Full screen width
  rollingText2->SetHeight(40);
  m_rollingText2 = rollingText2.get();
  m_rootContainer->AddWidget(std::move(rollingText2));
}

/// @brief Initialize the monitor menu container with 5 trading product displays
/// @details Creates a horizontal layout of 5 trading monitors, each displaying:
///          - Product material icon (64x64)
///          - Product name, quantity/max, and current price
///          - Trend arrows (up/down) based on price movement
///          - Clickable buttons to select products for detailed trading
///          Products: TRI (Tritanium), NFX (Neuroflux), ZER (Zeromass), LUM (Lumirite), NAN (Nanochip)
void ApplicationUI::UI_InitializeMonitorMenuContainer()
{
  // Create main container for all 5 trading monitors (positioned below title, above other UI)
  auto menuMonitorContainer = std::make_unique<ui::WidgetContainer>(0, 200, 1920, 380);
  menuMonitorContainer->SetLayout(ui::LayoutType::Native);  // Manual positioning for precise control
  m_monitorMenuContainer = menuMonitorContainer.get();     // Store raw pointer for later access
  m_rootContainer->AddWidget(std::move(menuMonitorContainer));

  // === Monitor 1: Tritanium Ore (TRI) ===
  uint16_t posX = 2;  // Small left margin
  auto monitor1Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor1Container->SetLayout(ui::LayoutType::Native);
  m_monitor1Container = monitor1Container.get();  // Store for adding icons/text later
  m_monitorMenuContainer->AddWidget(std::move(monitor1Container));

  // Clickable background button for Monitor 1 - selects Tritanium for trading
  auto ButtonMonitor1 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor1->LoadImage("BgMonitor.png");
  ButtonMonitor1->SetOnClickCallback([this](){
    if (m_application && m_application->m_stockMarket) {
      m_application->m_stockMarket->SetCurrentProductID("TRI"); // Select Tritanium Ore
    }
  });
  m_monitor1Container->AddWidget(std::move(ButtonMonitor1));

  // === Monitor 2: Neuroflux (NFX) ===
  const uint16_t gapX = 5;  // Spacing between monitors
  posX += 380 + posX;       // Position next to Monitor 1
  auto monitor2Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor2Container->SetLayout(ui::LayoutType::Native);
  m_monitor2Container = monitor2Container.get();
  m_monitorMenuContainer->AddWidget(std::move(monitor2Container));

  auto ButtonMonitor2 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor2->LoadImage("BgMonitor.png");
  ButtonMonitor2->SetOnClickCallback([this](){
    if (m_application && m_application->m_stockMarket) {
      m_application->m_stockMarket->SetCurrentProductID("NFX"); // Select Neuroflux
    }
  });
  m_monitor2Container->AddWidget(std::move(ButtonMonitor2));

  // === Monitor 3: Zeromass Matter (ZER) ===
  posX += 380 + gapX;
  auto monitor3Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor3Container->SetLayout(ui::LayoutType::Native);
  m_monitor3Container = monitor3Container.get();
  m_monitorMenuContainer->AddWidget(std::move(monitor3Container));

  auto ButtonMonitor3 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor3->LoadImage("BgMonitor.png");
  ButtonMonitor3->SetOnClickCallback([this](){
    if (m_application && m_application->m_stockMarket) {
      m_application->m_stockMarket->SetCurrentProductID("ZER"); // Select Zeromass Matter
    }
  });
  m_monitor3Container->AddWidget(std::move(ButtonMonitor3));

  // === Monitor 4: Lumirite Crystal (LUM) ===
  posX += 380 + gapX;
  auto monitor4Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor4Container->SetLayout(ui::LayoutType::Native);
  m_monitor4Container = monitor4Container.get(); // Store for adding icons/text later
  m_monitorMenuContainer->AddWidget(std::move(monitor4Container));

  auto ButtonMonitor4 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor4->LoadImage("BgMonitor.png");
  ButtonMonitor4->SetOnClickCallback([this](){
    if (m_application && m_application->m_stockMarket) {
      m_application->m_stockMarket->SetCurrentProductID("LUM"); // Select Lumirite Crystal
    }
  });
  m_monitor4Container->AddWidget(std::move(ButtonMonitor4));

  // === Monitor 5: Nanochip (NAN) ===
  posX += 380 + gapX;
  auto monitor5Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor5Container->SetLayout(ui::LayoutType::Native);
  m_monitor5Container = monitor5Container.get(); // Store for adding icons/text later
  m_monitorMenuContainer->AddWidget(std::move(monitor5Container));

  auto ButtonMonitor5 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor5->LoadImage("BgMonitor.png");
  ButtonMonitor5->SetOnClickCallback([this](){
    if (m_application && m_application->m_stockMarket) {
      m_application->m_stockMarket->SetCurrentProductID("NAN"); // Select Nanochip
    }
  });
  m_monitor5Container->AddWidget(std::move(ButtonMonitor5));
}

/// @brief Initialize the sub menu container with additional functionality buttons
/// @details Creates a vertical container on the left side with expandable menu buttons.
///          Currently contains placeholder buttons for future features like:
///          - Player inventory management
///          - Settings and configuration
///          - Trading history and statistics
///          Uses vertical layout with 50px spacing between buttons
void ApplicationUI::UI_InitializeSubMenuContainer()
{
  // Create left-side vertical menu container for additional game functions
  auto menuSubContainer = std::make_unique<ui::WidgetContainer>(50, 600, 500, 600);
  menuSubContainer->SetLayout(ui::LayoutType::Vertical, 50);  // Vertical stack with spacing
  m_subMenuContainer = menuSubContainer.get(); // Store for future expansion
  m_rootContainer->AddWidget(std::move(menuSubContainer));

  // === Placeholder buttons for future features ===
  // All positions are relative to the container (50, 600)

  auto subMenuButton1 = std::make_unique<ui::WidgetButton>(10, 10, 300, 100);
  subMenuButton1->LoadImage("BgInventory.png");
  subMenuButton1->SetOnClickCallback([](){ /* TODO: Open player inventory */ });
  m_subMenuContainer->AddWidget(std::move(subMenuButton1));

  auto subMenuButton2 = std::make_unique<ui::WidgetButton>(10, 120, 200, 100);
  subMenuButton2->LoadImage("BgInventory.png");
  subMenuButton2->SetOnClickCallback([](){ /* TODO: Open settings menu */ });
  m_subMenuContainer->AddWidget(std::move(subMenuButton2));

  auto subMenuButton3 = std::make_unique<ui::WidgetButton>(10, 230, 200, 100);
  subMenuButton3->LoadImage("BgInventory.png");
  subMenuButton3->SetOnClickCallback([](){ /* TODO: Open trading history */ });
  m_subMenuContainer->AddWidget(std::move(subMenuButton3));
}

/// @brief Initialize the game time display widget with digital font
/// @details Creates a digital clock display in the top-right corner showing elapsed game time.
///          Attempts to load a custom digital font for authentic look, falls back to default
///          font if loading fails. The text is right-aligned and updates in real-time.
///          Position: (1800, 100) - top right area of screen
void ApplicationUI::UI_InitializeGameTimeWidget()
{
  // Attempt to load custom digital font for game time display
  std::string fontPath = Application::s_assetsPath + "FontDigitalNumbers.ttf";
  if (m_digitalFont.loadFromFile(fontPath))
  {
    // Create game time text with digital font (preferred)
    auto gameTimeText = std::make_unique<ui::WidgetText>(1800, 100, "GametimeInitText");
    gameTimeText->SetCharacterSize(20);                                 // Medium size for readability
    gameTimeText->SetStyle(sf::Text::Bold);                             // Bold for emphasis
    gameTimeText->SetAlignment(ui::WidgetText::Alignment::Right);       // Right-align at X position
    gameTimeText->SetTextColor(sf::Color::White);                       // White for visibility
    gameTimeText->SetFont(m_digitalFont);                               // Use custom digital font
    m_gameTimeText = gameTimeText.get();
    m_rootContainer->AddWidget(std::move(gameTimeText));
  }
  else
  {
    // Fallback to default font if FontDigitalNumbers.ttf cannot be loaded
    auto gameTimeText = std::make_unique<ui::WidgetText>(1800, 100, "GametimeInitText");
    gameTimeText->SetCharacterSize(20);
    gameTimeText->SetStyle(sf::Text::Bold);
    gameTimeText->SetAlignment(ui::WidgetText::Alignment::Right);
    gameTimeText->SetTextColor(sf::Color::White);
    m_gameTimeText = gameTimeText.get();
    m_rootContainer->AddWidget(std::move(gameTimeText));
  }
}

/// @brief Initialize all image widgets including material icons and trend arrows
/// @details This function creates and positions visual elements for each trading monitor:
///          - Material icons (64x64) representing each tradeable resource
///          - Trend arrows (32x32) for up/down price movement indicators
///          - Text widgets for product name, quantity, and price information
///          - Sets up initial visibility states for trend arrows (alternating for testing)
///          All widgets are positioned relative to their parent monitor containers
void ApplicationUI::UI_InitializeImageWidgets()
{
  // === MATERIAL ICONS SECTION ===
  // Create 64x64 pixel material icons for each tradeable resource, positioned at (80,80) in each monitor
  auto iconTritanium = std::make_unique<ui::WidgetImage>(80, 60, 130, 130, "IconMaterialTritanium.png");
  m_iconMaterialTritanium = iconTritanium.get();
  m_monitor1Container->AddWidget(std::move(iconTritanium));

  auto iconNeuro = std::make_unique<ui::WidgetImage>(80, 60, 130, 130, "IconMaterialNeuro.png");
  m_iconMaterialNeuro = iconNeuro.get();
  m_monitor2Container->AddWidget(std::move(iconNeuro));

  auto iconZeromass = std::make_unique<ui::WidgetImage>(80, 60, 130, 130, "IconMaterialZeromass.png");
  m_iconMaterialZeromass = iconZeromass.get();
  m_monitor3Container->AddWidget(std::move(iconZeromass));

  auto iconLumi = std::make_unique<ui::WidgetImage>(80, 60, 130, 130, "IconMaterialLumi.png");
  m_iconMaterialLumi = iconLumi.get();
  m_monitor4Container->AddWidget(std::move(iconLumi));

  auto iconNano = std::make_unique<ui::WidgetImage>(80, 60, 130, 130, "IconMaterialNano.png");
  m_iconMaterialNano = iconNano.get();
  m_monitor5Container->AddWidget(std::move(iconNano));

  //---------------------------------------------------------------------------
  // Trend Images - Arrow indicators for up/down trends (32x32 pixels)
  // Create trend arrows for all monitor containers
  ui::WidgetContainer* monitorContainers[] = {
    m_monitor1Container, m_monitor2Container, m_monitor3Container,
    m_monitor4Container, m_monitor5Container
  };

  for (int i = 0; i < 5; ++i)
  {
    if (monitorContainers[i] != nullptr)
    {
      // Create trend down arrow for this monitor
      auto trendDown = std::make_unique<ui::WidgetImage>(230, 80, 80, 80, "ImageTrendArrowDown.png");
      m_imageTrendArrowDown[i] = trendDown.get();
      monitorContainers[i]->AddWidget(std::move(trendDown));

      // Create trend up arrow for this monitor
      auto trendUp = std::make_unique<ui::WidgetImage>(230, 80, 80, 80, "ImageTrendArrowUp.png");
      m_imageTrendArrowUp[i] = trendUp.get();
      monitorContainers[i]->AddWidget(std::move(trendUp));

      // Create text widgets for this monitor (positioned like icons: 80, 80 + offsets)
      auto txtProd = std::make_unique<ui::WidgetText>(190, 290, "TxtProd");
      txtProd->SetCharacterSize(22);
      txtProd->SetTextColor(sf::Color::White);
      txtProd->SetStyle(sf::Text::Bold);
      txtProd->SetAlignment(ui::WidgetText::Alignment::Center);
      txtProd->SetWidth(100);  // Explicit width
      txtProd->SetHeight(20);  // Explicit height
      m_txtProd[i] = txtProd.get();
      monitorContainers[i]->AddWidget(std::move(txtProd));
      // Container automatically calculates absolute position from relative position

      auto txtProdQuantity = std::make_unique<ui::WidgetText>(150, 220, "TxtQuantity");
      txtProdQuantity->SetCharacterSize(20);
      txtProdQuantity->SetTextColor(sf::Color::Yellow);
      txtProdQuantity->SetStyle(sf::Text::Bold);
      txtProdQuantity->SetAlignment(ui::WidgetText::Alignment::Center);
      txtProdQuantity->SetWidth(100);  // Explicit width
      txtProdQuantity->SetHeight(18);  // Explicit height
      m_txtProdQuantity[i] = txtProdQuantity.get();
      monitorContainers[i]->AddWidget(std::move(txtProdQuantity));
      // Container automatically calculates absolute position from relative position

      auto txtProdPrice = std::make_unique<ui::WidgetText>(270, 170, "TxtPrice");
      txtProdPrice->SetCharacterSize(20);
      txtProdPrice->SetTextColor(sf::Color::Green);
      txtProdPrice->SetStyle(sf::Text::Bold);
      txtProdPrice->SetAlignment(ui::WidgetText::Alignment::Center);
      txtProdPrice->SetWidth(100);  // Explicit width
      txtProdPrice->SetHeight(18);  // Explicit height
      m_txtProdPrice[i] = txtProdPrice.get();
      monitorContainers[i]->AddWidget(std::move(txtProdPrice));
      // Container automatically calculates absolute position from relative position

      // Update layout for this container after adding widgets
      monitorContainers[i]->UpdateLayout();
    }
  }

  // TEST visibility settings for trend arrows
  m_imageTrendArrowDown[0]->SetVisible(false);
  m_imageTrendArrowUp[0]->SetVisible(true);

  m_imageTrendArrowDown[1]->SetVisible(true);
  m_imageTrendArrowUp[1]->SetVisible(false);

  m_imageTrendArrowDown[2]->SetVisible(false);
  m_imageTrendArrowUp[2]->SetVisible(true);

  m_imageTrendArrowDown[3]->SetVisible(true);
  m_imageTrendArrowUp[3]->SetVisible(false);

  m_imageTrendArrowDown[4]->SetVisible(false);
  m_imageTrendArrowUp[4]->SetVisible(true);
}

/// @brief Enable/disable debug drawing for all containers and widgets
/// @details Adds colored borders around UI containers to help with layout debugging and development.
///          Each container type gets a different border color for easy identification:
///          - Root container: Red borders
///          - Monitor containers: Red borders (all 5 monitors)
///          - Sub menu container: Blue semi-transparent borders
///          Currently disabled by early return - enable for development by commenting out the return statement
void ApplicationUI::UI_DebugContainers()
{
  return; // Disable debug drawing for now - remove this line to enable debug borders

  // Enable debug borders for layout visualization (development only)

  // Root container debug (red border)
  m_rootContainer->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque

  // Main menu container debug (green border)
  m_monitorMenuContainer->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque

  // Monitor containers debug (different colors for each)
  if (m_monitor1Container)
    m_monitor1Container->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque
  if (m_monitor2Container)
    m_monitor2Container->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque
  if (m_monitor3Container)
    m_monitor3Container->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque
  if (m_monitor4Container)
    m_monitor4Container->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque
  if (m_monitor5Container)
    m_monitor5Container->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque
  // Enable debug for buttons in main menu
  for (size_t i = 0; i < m_monitorMenuContainer->GetWidgetCount(); ++i)
  {
    // Note: This would require a way to get child widgets by index
    // For now, buttons debug is handled during creation if needed
  }

  // Sub menu container debug (blue border)
  m_subMenuContainer->EnableDebugDraw(true, sf::Color(0, 0, 255, 128)); // Blue semi-transparent
}

// Initialize progress bars for health, energy, etc.
void ApplicationUI::UI_InitializeProgressBars()
{
  // Create Health Progress Bar (red theme) with "HP" suffix
  auto healthProgressBar = std::make_unique<ui::WidgetProgressBar>(50, 50, 300, 25, " HP");
  healthProgressBar->SetForegroundColor(sf::Color(220, 20, 20));  // Red
  healthProgressBar->SetBackgroundColor(sf::Color(64, 64, 64));   // Dark gray
  healthProgressBar->SetBorderColor(sf::Color::White);
  healthProgressBar->SetBorderThickness(2.0f);
  healthProgressBar->SetProgress(0.75f);  // 75% health
  healthProgressBar->SetShowPercentage(true);
  healthProgressBar->SetTextSize(12); // Smaller text

  // Set font if available
  if (m_digitalFont.getInfo().family != "")
  {
    healthProgressBar->SetFont(m_digitalFont);
  }  m_healthProgressBar = healthProgressBar.get();
  m_rootContainer->AddWidget(std::move(healthProgressBar));

  // Create Energy Progress Bar (blue theme) with "MP" suffix
  auto energyProgressBar = std::make_unique<ui::WidgetProgressBar>(50, 85, 300, 25, " MP");
  energyProgressBar->SetForegroundColor(sf::Color(20, 120, 220)); // Blue
  energyProgressBar->SetBackgroundColor(sf::Color(64, 64, 64));   // Dark gray
  energyProgressBar->SetBorderColor(sf::Color::White);
  energyProgressBar->SetBorderThickness(2.0f);
  energyProgressBar->SetProgress(0.45f);  // 45% energy
  energyProgressBar->SetShowPercentage(true);
  energyProgressBar->SetTextSize(12); // Smaller text

  // Set font if available
  if (m_digitalFont.getInfo().family != "")
  {
    energyProgressBar->SetFont(m_digitalFont);
  }  m_energyProgressBar = energyProgressBar.get();
  m_rootContainer->AddWidget(std::move(energyProgressBar));
}

/// @brief Update all product displays with current stock market data
/// @details This function refreshes all 5 trading monitors with real-time data from the stock market:
///          - Product names (TRI, NFX, ZER, LUM, NAN)
///          - Current quantity vs maximum quantity (e.g., "150/500")
///          - Current price with trend-based color coding
///          - Trend arrows (up/down) based on price movement direction
///
///          Color coding: Green text + up arrow = rising prices, Red text + down arrow = falling prices
///          Called automatically during UI initialization and by stock market cycle updates
void ApplicationUI::UpdateProductDisplays()
{
  // Safety check: ensure we have valid application and stock market references
  if (!m_application || !m_application->m_stockMarket)
    return;

  // Product IDs corresponding to each monitor (TRI=Tritanium, NFX=Neuroflux, ZER=Zeromass, LUM=Lumirite, NAN=Nanochip)
  std::string productIds[5] = { "TRI", "NFX", "ZER", "LUM", "NAN" };

  // Update each of the 5 trading monitors
  for (int i = 0; i < 5; ++i)
  {
    // Fetch current product data from stock market system
    StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[i]);

    // Ensure we have valid product data and all required text widgets exist
    if (product && m_txtProd[i] && m_txtProdQuantity[i] && m_txtProdPrice[i])
    {
      // Display product name (e.g., "Tritanium Ore", "Neuroflux")
      m_txtProd[i]->SetText(product->m_name);

      // Display current stock quantity vs maximum capacity (e.g., "150/500")
      std::string quantityText = std::to_string(product->m_quantity) + "/" + std::to_string(product->m_maxQuantity);
      m_txtProdQuantity[i]->SetText(quantityText);

      // Display current price (will be colored based on trend direction)
      std::string priceText = std::to_string(product->m_currentPrice);
      m_txtProdPrice[i]->SetText(priceText);

      // Update visual trend indicators based on price movement direction
      if (m_imageTrendArrowUp[i] && m_imageTrendArrowDown[i])
      {
        if (product->m_trendIncreased)
        {
          // Price is rising: show green text with up arrow
          m_imageTrendArrowUp[i]->SetVisible(true);
          m_imageTrendArrowDown[i]->SetVisible(false);
          m_txtProdPrice[i]->SetTextColor(sf::Color::Green);
        }
        else
        {
          // Price is falling: show red text with down arrow
          m_imageTrendArrowUp[i]->SetVisible(false);
          m_imageTrendArrowDown[i]->SetVisible(true);
          m_txtProdPrice[i]->SetTextColor(sf::Color::Red);
        }
      }
    }
  }
}
