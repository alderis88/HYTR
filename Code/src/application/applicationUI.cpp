#include "pch.h"
#include "application.h" // Aggregates required widget headers
#include "applicationUI.h"

ApplicationUI::ApplicationUI()
  : m_monitorMenuContainer(nullptr)
  , m_subMenuContainer(nullptr)
  , m_gameTimeText(nullptr)
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
  , m_imageTrendArrowDown(nullptr)
  , m_imageTrendArrowUp(nullptr)
{
}

// Main UI initialization - coordinates all container and widget creation
void ApplicationUI::InitializeUI()
{
  // Create containers and their widgets
  InitializeContainersUI();

  // Final layout update after full hierarchy established
  m_rootContainer->UpdateLayout();
}

// Creates all containers and calls their individual initialization functions
void ApplicationUI::InitializeContainersUI()
{
  // Initialize root and all child containers
  UI_InitializeRootContainer();
  UI_InitializeMonitorMenuContainer();
  UI_InitializeSubMenuContainer();
  UI_InitializeGameTimeWidget();
  UI_InitializeImageWidgets();

  // Enable debug drawing for all containers
  UI_DebugContainers();
}



// Initialize root container
void ApplicationUI::UI_InitializeRootContainer()
{
  //---------------------------------------------------------------------------
  // Root container - FULL SCREEN
  m_rootContainer = std::make_unique<ui::WidgetContainer>(0, 0, 1920, 1080);

  // Background - FULL SCREEN
  auto imageWidget = std::make_unique<ui::WidgetImage>(0, 0, 1920, 1080, "BgInit.png");
  m_rootContainer->AddWidget(std::move(imageWidget));

  // Title text (above background, below containers)
  auto titleText = std::make_unique<ui::WidgetText>(960, 100, "Hyper Trade");
  titleText->SetCharacterSize(30);
  titleText->SetStyle(sf::Text::Bold);
  titleText->SetAlignment(ui::WidgetText::Alignment::Center);
  titleText->SetTextColor(sf::Color::White);
  m_rootContainer->AddWidget(std::move(titleText));
}

// Initialize monitor menu container
void ApplicationUI::UI_InitializeMonitorMenuContainer()
{
  //---------------------------------------------------------------------------
  // Monitor menu container
  auto menuMonitorContainer = std::make_unique<ui::WidgetContainer>(0, 200, 1920, 380);
  //menuMonitorContainer->SetLayout(ui::LayoutType::Horizontal, 5);
  menuMonitorContainer->SetLayout(ui::LayoutType::Native);
  m_monitorMenuContainer = menuMonitorContainer.get(); // Store raw pointer
  m_rootContainer->AddWidget(std::move(menuMonitorContainer));

  // Create monitor1Container and  ButtonMonitor1
    uint16_t posX = 2;
  auto monitor1Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor1Container->SetLayout(ui::LayoutType::Native);
  m_monitor1Container = monitor1Container.get(); // Store raw pointer
  m_monitorMenuContainer->AddWidget(std::move(monitor1Container));

  auto ButtonMonitor1 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor1->LoadImage("BgMonitor.png");
  ButtonMonitor1->SetOnClickCallback([](){ /* Button click callback */ });
  m_monitor1Container->AddWidget(std::move(ButtonMonitor1));

  // Create monitor2Container and ButtonMonitor2
  const uint16_t gapX = 5;
  posX += 380 + posX;
  auto monitor2Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor2Container->SetLayout(ui::LayoutType::Native);
  m_monitor2Container = monitor2Container.get();
  m_monitorMenuContainer->AddWidget(std::move(monitor2Container));

  auto ButtonMonitor2 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor2->LoadImage("BgMonitor.png");
  ButtonMonitor2->SetOnClickCallback([](){ /* Button 2 click callback */ });
  m_monitor2Container->AddWidget(std::move(ButtonMonitor2));

  // Create monitor3Container and ButtonMonitor3
  posX += 380 + gapX;
  auto monitor3Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor3Container->SetLayout(ui::LayoutType::Native);
  m_monitor3Container = monitor3Container.get();
  m_monitorMenuContainer->AddWidget(std::move(monitor3Container));

  auto ButtonMonitor3 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor3->LoadImage("BgMonitor.png");
  ButtonMonitor3->SetOnClickCallback([](){ /* Button 3 click callback */ });
  m_monitor3Container->AddWidget(std::move(ButtonMonitor3));

  // Create monitor4Container and ButtonMonitor4
  posX += 380 + gapX;
  auto monitor4Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor4Container->SetLayout(ui::LayoutType::Native);
  m_monitor4Container = monitor4Container.get(); // Store raw pointer
  m_monitorMenuContainer->AddWidget(std::move(monitor4Container));

  auto ButtonMonitor4 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor4->LoadImage("BgMonitor.png");
  ButtonMonitor4->SetOnClickCallback([](){ /* Button 4 click callback */ });
  m_monitor4Container->AddWidget(std::move(ButtonMonitor4));

  // Create monitor5Container and ButtonMonitor5
  posX += 380 + gapX;
  auto monitor5Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
  monitor5Container->SetLayout(ui::LayoutType::Native);
  m_monitor5Container = monitor5Container.get(); // Store raw pointer
  m_monitorMenuContainer->AddWidget(std::move(monitor5Container));

  auto ButtonMonitor5 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
  ButtonMonitor5->LoadImage("BgMonitor.png");
  ButtonMonitor5->SetOnClickCallback([](){ /* Button 5 click callback */ });
  m_monitor5Container->AddWidget(std::move(ButtonMonitor5));
}

// Initialize sub menu container
void ApplicationUI::UI_InitializeSubMenuContainer()
{
  //---------------------------------------------------------------------------
  // Sub menu container
  auto menuSubContainer = std::make_unique<ui::WidgetContainer>(50, 500, 500, 600);
  menuSubContainer->SetLayout(ui::LayoutType::Vertical, 50);
  m_subMenuContainer = menuSubContainer.get(); // Store raw pointer
  m_rootContainer->AddWidget(std::move(menuSubContainer));

  // Container already initialized by UI_InitializeRootContainer()
  // Use RELATIVE positions since AddWidget will add container position
  auto subMenuButton1 = std::make_unique<ui::WidgetButton>(10, 10, 300, 100);
  subMenuButton1->LoadImage("BgMonitor.png");
  subMenuButton1->SetOnClickCallback([](){ /* Sub Button 1 click */ });
  m_subMenuContainer->AddWidget(std::move(subMenuButton1));

  auto subMenuButton2 = std::make_unique<ui::WidgetButton>(10, 120, 200, 100);
  subMenuButton2->LoadImage("BgMonitor.png");
  subMenuButton2->SetOnClickCallback([](){ /* Sub Button 2 click */ });
  m_subMenuContainer->AddWidget(std::move(subMenuButton2));

  auto subMenuButton3 = std::make_unique<ui::WidgetButton>(10, 230, 200, 100);
  subMenuButton3->LoadImage("BgMonitor.png");
  subMenuButton3->SetOnClickCallback([](){ /* Sub Button 3 click */ });
  m_subMenuContainer->AddWidget(std::move(subMenuButton3));
}

// Initialize game time display widget
void ApplicationUI::UI_InitializeGameTimeWidget()
{
  // Load the digital numbers font
  std::string fontPath = Application::s_assetsPath + "FontDigitalNumbers.ttf";
  if (m_digitalFont.loadFromFile(fontPath))
  {
    auto gameTimeText = std::make_unique<ui::WidgetText>(1800, 100, "GametimeInitText");
    gameTimeText->SetCharacterSize(20);
    gameTimeText->SetStyle(sf::Text::Bold);
    gameTimeText->SetAlignment(ui::WidgetText::Alignment::Right);
    gameTimeText->SetTextColor(sf::Color::White);
    gameTimeText->SetFont(m_digitalFont);
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

// Initialize Image Widgets for Icons and Images
void ApplicationUI::UI_InitializeImageWidgets()
{
  //---------------------------------------------------------------------------
  // Material Icons - Small icons for different materials (64x64 pixels)
  auto iconLumi = std::make_unique<ui::WidgetImage>(100, 200, 64, 64, "IconMaterialLumi.png");
  m_iconMaterialLumi = iconLumi.get();
  m_rootContainer->AddWidget(std::move(iconLumi));

  auto iconNano = std::make_unique<ui::WidgetImage>(170, 200, 64, 64, "IconMaterialNano.png");
  m_iconMaterialNano = iconNano.get();
  m_rootContainer->AddWidget(std::move(iconNano));

  auto iconNeuro = std::make_unique<ui::WidgetImage>(240, 200, 64, 64, "IconMaterialNeuro.png");
  m_iconMaterialNeuro = iconNeuro.get();
  m_rootContainer->AddWidget(std::move(iconNeuro));

  auto iconTritanium = std::make_unique<ui::WidgetImage>(310, 200, 64, 64, "IconMaterialTritanium.png");
  m_iconMaterialTritanium = iconTritanium.get();
  m_rootContainer->AddWidget(std::move(iconTritanium));

  auto iconZeromass = std::make_unique<ui::WidgetImage>(380, 200, 64, 64, "IconMaterialZeromass.png");
  m_iconMaterialZeromass = iconZeromass.get();
  m_rootContainer->AddWidget(std::move(iconZeromass));

  //---------------------------------------------------------------------------
  // Trend Images - Arrow indicators for up/down trends (32x32 pixels)
  auto trendDown = std::make_unique<ui::WidgetImage>(500, 300, 32, 32, "ImageTrendArrowDown.png");
  m_imageTrendArrowDown = trendDown.get();
  m_rootContainer->AddWidget(std::move(trendDown));

  auto trendUp = std::make_unique<ui::WidgetImage>(540, 300, 32, 32, "ImageTrendArrowUp.png");
  m_imageTrendArrowUp = trendUp.get();
  m_rootContainer->AddWidget(std::move(trendUp));
}

// Enable/disable debug drawing for all containers and widgets
void ApplicationUI::UI_DebugContainers()
{
  // All containers already initialized by UI_InitializeRootContainer()

  // Root container debug (red border)
  m_rootContainer->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque

  // Main menu container debug (green border)
  m_monitorMenuContainer->EnableDebugDraw(true, sf::Color(0, 255, 0, 128)); // Green semi-transparent

  // Monitor containers debug (different colors for each)
  if (m_monitor1Container)
    m_monitor1Container->EnableDebugDraw(true, sf::Color(255, 255, 0, 128)); // Yellow semi-transparent
  if (m_monitor2Container)
    m_monitor2Container->EnableDebugDraw(true, sf::Color(255, 0, 255, 128)); // Magenta semi-transparent
  if (m_monitor3Container)
    m_monitor3Container->EnableDebugDraw(true, sf::Color(0, 255, 255, 128)); // Cyan semi-transparent
  if (m_monitor4Container)
    m_monitor4Container->EnableDebugDraw(true, sf::Color(255, 128, 0, 128)); // Orange semi-transparent
  if (m_monitor5Container)
    m_monitor5Container->EnableDebugDraw(true, sf::Color(128, 255, 0, 128)); // Light green semi-transparent

  // Enable debug for buttons in main menu
  for (size_t i = 0; i < m_monitorMenuContainer->GetWidgetCount(); ++i)
  {
    // Note: This would require a way to get child widgets by index
    // For now, buttons debug is handled during creation if needed
  }

  // Sub menu container debug (blue border)
  m_subMenuContainer->EnableDebugDraw(true, sf::Color(0, 0, 255, 128)); // Blue semi-transparent
}
