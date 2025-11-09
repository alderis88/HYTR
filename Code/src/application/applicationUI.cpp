/**
 * @file applicationUI.cpp
 * @brief Implementation of the ApplicationUI class for managing the main game UI
 * @details Handles creation and management of all UI widgets including trading monitors,
 *          progress bars, game time display, and product information displays.
 */

#include "pch.h"
#include "application.h" // Aggregates required widget headers
#include "applicationUI.h"
#include <sstream>
#include <iomanip>
#include "stockMarket.h" // Required for SetCurrentProductID method
#include "inventory.h" // Required for StockProduct structure

 /// @brief Default constructor for ApplicationUI
 /// @details Initializes all member pointers to nullptr and sets up widget arrays for safe access
ApplicationUI::ApplicationUI()
	: m_monitorMenuContainer(nullptr)
	, m_tradeContainer(nullptr)
	, m_inventoryContainer(nullptr)
	, m_productInfoContainer(nullptr)
	, m_companyInfoContainer(nullptr)
	, m_vendorInfoContainer(nullptr)
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
	, m_logoImage(nullptr)
	, m_cycleProgressBar(nullptr)
	, m_energyProgressBar(nullptr)
	, m_volumeText(nullptr)
	, m_volumeProgressBar(nullptr)
	, m_inventoryProductsContainer(nullptr)
	, m_triInvContainer(nullptr)
	, m_nfxInvContainer(nullptr)
	, m_zerInvContainer(nullptr)
	, m_lumInvContainer(nullptr)
	, m_nanInvContainer(nullptr)
	, m_triInvButton(nullptr)
	, m_nfxInvButton(nullptr)
	, m_zerInvButton(nullptr)
	, m_lumInvButton(nullptr)
	, m_nanInvButton(nullptr)
	, m_triInvIcon(nullptr)
	, m_nfxInvIcon(nullptr)
	, m_zerInvIcon(nullptr)
	, m_lumInvIcon(nullptr)
	, m_nanInvIcon(nullptr)
	, m_triInvQuantityText(nullptr)
	, m_nfxInvQuantityText(nullptr)
	, m_zerInvQuantityText(nullptr)
	, m_lumInvQuantityText(nullptr)
	, m_nanInvQuantityText(nullptr)
	, m_triInvVolumeText(nullptr)
	, m_nfxInvVolumeText(nullptr)
	, m_zerInvVolumeText(nullptr)
	, m_lumInvVolumeText(nullptr)
	, m_nanInvVolumeText(nullptr)
	, m_productInfoImage(nullptr)
	, m_productInfoText(nullptr)
	, m_productVolumeText(nullptr)
	, m_productRarityText(nullptr)
	, m_vendorImage(nullptr)
	, m_vendorNameText(nullptr)
	, m_vendorProfileText(nullptr)
	, m_companyNameText(nullptr)
	, m_companyInfoText(nullptr)
	, m_companyLogo(nullptr)
	, m_vendorRoleText(nullptr)
	, m_productNameText(nullptr)
	, m_confirmTradeButton(nullptr)
	, m_cancelTradeButton(nullptr)
	, m_currentMoneyText(nullptr)
	, m_infoPanelSelectorContainer(nullptr)
	, m_productInfoSelectorButton(nullptr)
	, m_companyInfoSelectorButton(nullptr)
	, m_vendorInfoSelectorButton(nullptr)
	, m_selectedInfoPanel(0) // Default to Product info panel
	, m_inventorySortSelectorContainer(nullptr)
	, m_volumeSortButton(nullptr)
	, m_quantitySortButton(nullptr)
	, m_selectedSortType(InventorySortType::Volume) // Default to Volume sort
	, m_rollingText1Position(1920.0f) // Start off-screen to the right
	, m_rollingText2Position(3940.0f) // Start off-screen with offset (1920 + 960)
	, m_rollingSpeed(100.0f) // 100 pixels per second
	, m_selectedMonitorIndex(-1) // No monitor selected initially
{
	// Initialize all widget arrays to nullptr for safe access checks later
	for (int i = 0; i < 5; ++i)
	{
		m_imageTrendArrowDown[i] = nullptr;
		m_imageTrendArrowUp[i] = nullptr;
		m_txtProd[i] = nullptr;
		m_txtProdQuantity[i] = nullptr;
		m_txtProdPrice[i] = nullptr;
		m_monitorButtons[i] = nullptr;
		m_monitorHighlights[i] = nullptr;
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

	// Update current money display
	UpdateCurrentMoneyDisplay();

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
	UI_InitializeGameTimeWidget();         // Digital time display with custom font (loads fonts first)
	UI_InitializeLogo();                   // Logo in top left corner
	UI_InitializeMonitorMenuContainer();   // 5 trading monitor displays with click handlers
	UI_InitializeTradeContainer();         // Trade confirm/cancel buttons
	UI_InitializeInventoryContainer();     // Inventory container (bottom left)
	UI_InitializeInventorySortSelector();  // Inventory sort selector buttons (above inventory container)
	UI_InitializeProductInfoContainer();   // Product info container (bottom right)
	UI_InitializeCompanyInfoContainer();   // Company info container (bottom right)
	UI_InitializeVendorInfoContainer();    // Vendor info container (bottom right)
	UI_InitializeInfoPanelSelector();      // Info panel selector buttons (above info containers)
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

	std::string fontPath = Application::s_assetsPath + "FontLedNews.ttf";
	if (m_ledFont.loadFromFile(fontPath))
	{

		// Add rolling text 1 - full width at very top of screen
		auto rollingText1 = std::make_unique<ui::WidgetText>(0, 50, "Loading news...");
		rollingText1->SetCharacterSize(50);
		rollingText1->SetFont(m_ledFont); // Use led font for rolling text 1
		rollingText1->SetStyle(sf::Text::Regular);
		rollingText1->SetAlignment(ui::WidgetText::Alignment::Left);
		rollingText1->SetTextColor(sf::Color::Red);
		rollingText1->SetWidth(1920);  // Full screen width
		rollingText1->SetHeight(100);
		m_rollingText1 = rollingText1.get();
		m_rootContainer->AddWidget(std::move(rollingText1));

		// Add rolling text 2 - full width below the first one
		auto rollingText2 = std::make_unique<ui::WidgetText>(1920, 20, "Loading news...2");
		rollingText2->SetCharacterSize(50);
		rollingText2->SetFont(m_ledFont); // Use led font for rolling text 2
		rollingText2->SetStyle(sf::Text::Regular);
		rollingText2->SetAlignment(ui::WidgetText::Alignment::Left);
		rollingText2->SetTextColor(sf::Color::Red);
		rollingText2->SetWidth(1920);  // Full screen width
		rollingText2->SetHeight(100);
		m_rollingText2 = rollingText2.get();
		m_rootContainer->AddWidget(std::move(rollingText2));
	}
	// Load initial news content for rolling text
	LoadingNewText(true);
	LoadingNewText(false);
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
	ButtonMonitor1->SetOnClickCallback([this]() {
		SelectMonitor(0); // Call SelectMonitor instead of direct product ID setting
		});
	m_monitorButtons[0] = ButtonMonitor1.get(); // Store button reference
	m_monitor1Container->AddWidget(std::move(ButtonMonitor1));

	// Add purple highlight overlay (initially hidden)
	auto highlight1 = std::make_unique<ui::WidgetImage>(0, 0, 380, 340, "BgMonitorSelected.png");
	highlight1->SetVisible(false); // Initially hidden
	m_monitorHighlights[0] = highlight1.get();
	m_monitor1Container->AddWidget(std::move(highlight1));

	// === Monitor 2: Neuroflux (NFX) ===
	const uint16_t gapX = 5;  // Spacing between monitors
	posX += 380 + posX;       // Position next to Monitor 1
	auto monitor2Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
	monitor2Container->SetLayout(ui::LayoutType::Native);
	m_monitor2Container = monitor2Container.get();
	m_monitorMenuContainer->AddWidget(std::move(monitor2Container));

	auto ButtonMonitor2 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
	ButtonMonitor2->LoadImage("BgMonitor.png");
	ButtonMonitor2->SetOnClickCallback([this]() {
		SelectMonitor(1); // Call SelectMonitor instead of direct product ID setting
		});
	m_monitorButtons[1] = ButtonMonitor2.get(); // Store button reference
	m_monitor2Container->AddWidget(std::move(ButtonMonitor2));

	// Add purple highlight overlay (initially hidden)
	auto highlight2 = std::make_unique<ui::WidgetImage>(0, 0, 380, 340, "BgMonitorSelected.png");
	highlight2->SetVisible(false); // Initially hidden
	m_monitorHighlights[1] = highlight2.get();
	m_monitor2Container->AddWidget(std::move(highlight2));

	// === Monitor 3: Zeromass Matter (ZER) ===
	posX += 380 + gapX;
	auto monitor3Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
	monitor3Container->SetLayout(ui::LayoutType::Native);
	m_monitor3Container = monitor3Container.get();
	m_monitorMenuContainer->AddWidget(std::move(monitor3Container));

	auto ButtonMonitor3 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
	ButtonMonitor3->LoadImage("BgMonitor.png");
	ButtonMonitor3->SetOnClickCallback([this]() {
		SelectMonitor(2); // Call SelectMonitor instead of direct product ID setting
		});
	m_monitorButtons[2] = ButtonMonitor3.get(); // Store button reference
	m_monitor3Container->AddWidget(std::move(ButtonMonitor3));

	// Add purple highlight overlay (initially hidden)
	auto highlight3 = std::make_unique<ui::WidgetImage>(0, 0, 380, 340, "BgMonitorSelected.png");
	highlight3->SetVisible(false); // Initially hidden
	m_monitorHighlights[2] = highlight3.get();
	m_monitor3Container->AddWidget(std::move(highlight3));

	// === Monitor 4: Lumirite Crystal (LUM) ===
	posX += 380 + gapX;
	auto monitor4Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
	monitor4Container->SetLayout(ui::LayoutType::Native);
	m_monitor4Container = monitor4Container.get(); // Store for adding icons/text later
	m_monitorMenuContainer->AddWidget(std::move(monitor4Container));

	auto ButtonMonitor4 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
	ButtonMonitor4->LoadImage("BgMonitor.png");
	ButtonMonitor4->SetOnClickCallback([this]() {
		SelectMonitor(3); // Call SelectMonitor instead of direct product ID setting
		});
	m_monitorButtons[3] = ButtonMonitor4.get(); // Store button reference
	m_monitor4Container->AddWidget(std::move(ButtonMonitor4));

	// Add purple highlight overlay (initially hidden)
	auto highlight4 = std::make_unique<ui::WidgetImage>(0, 0, 380, 340, "BgMonitorSelected.png");
	highlight4->SetVisible(false); // Initially hidden
	m_monitorHighlights[3] = highlight4.get();
	m_monitor4Container->AddWidget(std::move(highlight4));

	// === Monitor 5: Nanochip (NAN) ===
	posX += 380 + gapX;
	auto monitor5Container = std::make_unique<ui::WidgetContainer>(posX, 0, 380, 340);
	monitor5Container->SetLayout(ui::LayoutType::Native);
	m_monitor5Container = monitor5Container.get(); // Store for adding icons/text later
	m_monitorMenuContainer->AddWidget(std::move(monitor5Container));

	auto ButtonMonitor5 = std::make_unique<ui::WidgetButton>(0, 0, 380, 340);
	ButtonMonitor5->LoadImage("BgMonitor.png");
	ButtonMonitor5->SetOnClickCallback([this]() {
		SelectMonitor(4); // Call SelectMonitor instead of direct product ID setting
		});
	m_monitorButtons[4] = ButtonMonitor5.get(); // Store button reference
	m_monitor5Container->AddWidget(std::move(ButtonMonitor5));

	// Add purple highlight overlay (initially hidden)
	auto highlight5 = std::make_unique<ui::WidgetImage>(0, 0, 380, 340, "BgMonitorSelected.png");
	highlight5->SetVisible(false); // Initially hidden
	m_monitorHighlights[4] = highlight5.get();
	m_monitor5Container->AddWidget(std::move(highlight5));
}

/// @brief Initialize the trade container with confirm and cancel trade buttons
/// @details Creates a horizontal container in the center of the screen with two trading action buttons.
///          Used for confirming or canceling trading operations when a monitor is selected.
///          Position: center of screen with horizontal layout
void ApplicationUI::UI_InitializeTradeContainer()
{
	// Create trade container for action buttons (positioned in middle of screen)
	auto tradeContainer = std::make_unique<ui::WidgetContainer>(600, 600, 720, 720);
	tradeContainer->SetLayout(ui::LayoutType::Native, 20); // Horizontal layout with 20px spacing
	m_tradeContainer = tradeContainer.get();
	m_rootContainer->AddWidget(std::move(tradeContainer));

	// === Confirm Trade Button ===
	auto confirmTradeButton = std::make_unique<ui::WidgetButton>(220, 200, 300, 100);
	confirmTradeButton->LoadImage("ButtonMain2.png");

	// Set font for button text BEFORE setting text (use digital font if available)
	if (m_digitalFont.getInfo().family != "") {
		confirmTradeButton->SetFont(m_digitalFont);
	}

	confirmTradeButton->SetText("CONFIRM");
	confirmTradeButton->SetTextColor(sf::Color::White);

	confirmTradeButton->SetOnClickCallback([this]() {
		// TODO: Handle confirm trade action based on current selection
		if (m_selectedMonitorIndex >= 0) {
			// Confirm the selected monitor trading action
			// Implementation depends on specific game logic requirements
		}
		});
	m_confirmTradeButton = confirmTradeButton.get();
	m_tradeContainer->AddWidget(std::move(confirmTradeButton));

	// === Cancel Trade Button ===
	auto cancelTradeButton = std::make_unique<ui::WidgetButton>(220, 300, 300, 100);
	cancelTradeButton->LoadImage("ButtonMain2.png");

	// Set font for button text BEFORE setting text (use digital font if available)
	if (m_digitalFont.getInfo().family != "") {
		cancelTradeButton->SetFont(m_digitalFont);
	}

	cancelTradeButton->SetText("CANCEL");
	cancelTradeButton->SetTextColor(sf::Color::White);

	cancelTradeButton->SetOnClickCallback([this]() {
		// Cancel current trade selection and return to default state
		CancelSelection();
		});
	m_cancelTradeButton = cancelTradeButton.get();
	m_tradeContainer->AddWidget(std::move(cancelTradeButton));

	// === Current Money Display ===
	auto currentMoneyText = std::make_unique<ui::WidgetText>(360, 150, "Money: $0");
	currentMoneyText->SetCharacterSize(24);
	currentMoneyText->SetStyle(sf::Text::Bold);
	currentMoneyText->SetAlignment(ui::WidgetText::Alignment::Center);
	currentMoneyText->SetTextColor(sf::Color::Yellow);
	
	// Set font if available
	if (m_digitalFont.getInfo().family != "") {
		currentMoneyText->SetFont(m_digitalFont);
	}
	
	m_currentMoneyText = currentMoneyText.get();
	m_tradeContainer->AddWidget(std::move(currentMoneyText));

	// Initially hide trade container until a monitor is selected
	m_tradeContainer->SetVisible(false);
}

/// @brief Initialize the inventory container in the bottom left corner
/// @details Creates a container for inventory management UI elements such as:
///          - Player inventory items
///          - Item quantities and descriptions
///          - Drag and drop functionality for trading
///          Position: bottom left corner of the screen
void ApplicationUI::UI_InitializeInventoryContainer()
{
	// Create inventory container in bottom left corner (400x300 pixels)
	auto inventoryContainer = std::make_unique<ui::WidgetContainer>(30, 600, 550, 400);
	inventoryContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
	m_inventoryContainer = inventoryContainer.get(); // Store raw pointer for later access
	m_rootContainer->AddWidget(std::move(inventoryContainer));

	// Add background for inventory container
	auto inventoryBackground = std::make_unique<ui::WidgetImage>(0, 0, 550, 400, "BgInventory.png");
	m_inventoryContainer->AddWidget(std::move(inventoryBackground));

	// Add inventory title text
	auto inventoryTitle = std::make_unique<ui::WidgetText>(270, 35, "INVENTORY");
	inventoryTitle->SetCharacterSize(24);
	inventoryTitle->SetStyle(sf::Text::Bold);
	inventoryTitle->SetAlignment(ui::WidgetText::Alignment::Center);
	inventoryTitle->SetTextColor(sf::Color::White);
	m_inventoryContainer->AddWidget(std::move(inventoryTitle));

	// Create vertical container for inventory products (5 product buttons)
	auto inventoryProductsContainer = std::make_unique<ui::WidgetContainer>(10, 70, 530, 250);
	inventoryProductsContainer->SetLayout(ui::LayoutType::Vertical, 5); // Vertical layout with 5px spacing
	m_inventoryProductsContainer = inventoryProductsContainer.get();
	m_inventoryContainer->AddWidget(std::move(inventoryProductsContainer));

	// === Tritanium Inventory Button Container ===
	auto triInvContainer = std::make_unique<ui::WidgetContainer>(0, 0, 530, 45);
	triInvContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
	m_triInvContainer = triInvContainer.get();
	m_inventoryProductsContainer->AddWidget(std::move(triInvContainer));

	// Tritanium inventory button (full width)
	auto triInvButton = std::make_unique<ui::WidgetButton>(0, 0, 530, 45);
	triInvButton->LoadImage("ButtonMain2.png");
	triInvButton->SetOnClickCallback([this]() {
		// TODO: Handle Tritanium inventory action
		});
	m_triInvButton = triInvButton.get();
	m_triInvContainer->AddWidget(std::move(triInvButton));

	// Tritanium icon
	auto triInvIcon = std::make_unique<ui::WidgetImage>(5, 7, 30, 30, "IconMaterialTritanium.png");
	m_triInvIcon = triInvIcon.get();
	m_triInvContainer->AddWidget(std::move(triInvIcon));

	// Tritanium quantity text
	auto triInvQuantityText = std::make_unique<ui::WidgetText>(45, 12, "Qty: 0");
	triInvQuantityText->SetCharacterSize(14);
	triInvQuantityText->SetTextColor(sf::Color::Yellow);
	triInvQuantityText->SetStyle(sf::Text::Bold);
	m_triInvQuantityText = triInvQuantityText.get();
	m_triInvContainer->AddWidget(std::move(triInvQuantityText));

	// Tritanium volume text
	auto triInvVolumeText = std::make_unique<ui::WidgetText>(150, 12, "Volume: 0");
	triInvVolumeText->SetCharacterSize(14);
	triInvVolumeText->SetTextColor(sf::Color::Cyan);
	triInvVolumeText->SetStyle(sf::Text::Bold);
	m_triInvVolumeText = triInvVolumeText.get();
	m_triInvContainer->AddWidget(std::move(triInvVolumeText));

	// === Neuroflux Inventory Button Container ===
	auto nfxInvContainer = std::make_unique<ui::WidgetContainer>(0, 0, 530, 45);
	nfxInvContainer->SetLayout(ui::LayoutType::Native);
	m_nfxInvContainer = nfxInvContainer.get();
	m_inventoryProductsContainer->AddWidget(std::move(nfxInvContainer));

	// Neuroflux inventory button
	auto nfxInvButton = std::make_unique<ui::WidgetButton>(0, 0, 530, 45);
	nfxInvButton->LoadImage("ButtonMain2.png");
	nfxInvButton->SetOnClickCallback([this]() {
		// TODO: Handle Neuroflux inventory action
		});
	m_nfxInvButton = nfxInvButton.get();
	m_nfxInvContainer->AddWidget(std::move(nfxInvButton));

	// Neuroflux icon
	auto nfxInvIcon = std::make_unique<ui::WidgetImage>(5, 7, 30, 30, "IconMaterialNeuro.png");
	m_nfxInvIcon = nfxInvIcon.get();
	m_nfxInvContainer->AddWidget(std::move(nfxInvIcon));

	// Neuroflux quantity text
	auto nfxInvQuantityText = std::make_unique<ui::WidgetText>(45, 12, "Qty: 0");
	nfxInvQuantityText->SetCharacterSize(14);
	nfxInvQuantityText->SetTextColor(sf::Color::Yellow);
	nfxInvQuantityText->SetStyle(sf::Text::Bold);
	m_nfxInvQuantityText = nfxInvQuantityText.get();
	m_nfxInvContainer->AddWidget(std::move(nfxInvQuantityText));

	// Neuroflux volume text
	auto nfxInvVolumeText = std::make_unique<ui::WidgetText>(150, 12, "Volume: 0");
	nfxInvVolumeText->SetCharacterSize(14);
	nfxInvVolumeText->SetTextColor(sf::Color::Cyan);
	nfxInvVolumeText->SetStyle(sf::Text::Bold);
	m_nfxInvVolumeText = nfxInvVolumeText.get();
	m_nfxInvContainer->AddWidget(std::move(nfxInvVolumeText));

	// === Zeromass Inventory Button Container ===
	auto zerInvContainer = std::make_unique<ui::WidgetContainer>(0, 0, 530, 45);
	zerInvContainer->SetLayout(ui::LayoutType::Native);
	m_zerInvContainer = zerInvContainer.get();
	m_inventoryProductsContainer->AddWidget(std::move(zerInvContainer));

	// Zeromass inventory button
	auto zerInvButton = std::make_unique<ui::WidgetButton>(0, 0, 530, 45);
	zerInvButton->LoadImage("ButtonMain2.png");
	zerInvButton->SetOnClickCallback([this]() {
		// TODO: Handle Zeromass inventory action
		});
	m_zerInvButton = zerInvButton.get();
	m_zerInvContainer->AddWidget(std::move(zerInvButton));

	// Zeromass icon
	auto zerInvIcon = std::make_unique<ui::WidgetImage>(5, 7, 30, 30, "IconMaterialZeromass.png");
	m_zerInvIcon = zerInvIcon.get();
	m_zerInvContainer->AddWidget(std::move(zerInvIcon));

	// Zeromass quantity text
	auto zerInvQuantityText = std::make_unique<ui::WidgetText>(45, 12, "Qty: 0");
	zerInvQuantityText->SetCharacterSize(14);
	zerInvQuantityText->SetTextColor(sf::Color::Yellow);
	zerInvQuantityText->SetStyle(sf::Text::Bold);
	m_zerInvQuantityText = zerInvQuantityText.get();
	m_zerInvContainer->AddWidget(std::move(zerInvQuantityText));

	// Zeromass volume text
	auto zerInvVolumeText = std::make_unique<ui::WidgetText>(150, 12, "Volume: 0");
	zerInvVolumeText->SetCharacterSize(14);
	zerInvVolumeText->SetTextColor(sf::Color::Cyan);
	zerInvVolumeText->SetStyle(sf::Text::Bold);
	m_zerInvVolumeText = zerInvVolumeText.get();
	m_zerInvContainer->AddWidget(std::move(zerInvVolumeText));

	// === Lumirite Inventory Button Container ===
	auto lumInvContainer = std::make_unique<ui::WidgetContainer>(0, 0, 530, 45);
	lumInvContainer->SetLayout(ui::LayoutType::Native);
	m_lumInvContainer = lumInvContainer.get();
	m_inventoryProductsContainer->AddWidget(std::move(lumInvContainer));

	// Lumirite inventory button
	auto lumInvButton = std::make_unique<ui::WidgetButton>(0, 0, 530, 45);
	lumInvButton->LoadImage("ButtonMain2.png");
	lumInvButton->SetOnClickCallback([this]() {
		// TODO: Handle Lumirite inventory action
		});
	m_lumInvButton = lumInvButton.get();
	m_lumInvContainer->AddWidget(std::move(lumInvButton));

	// Lumirite icon
	auto lumInvIcon = std::make_unique<ui::WidgetImage>(5, 7, 30, 30, "IconMaterialLumi.png");
	m_lumInvIcon = lumInvIcon.get();
	m_lumInvContainer->AddWidget(std::move(lumInvIcon));

	// Lumirite quantity text
	auto lumInvQuantityText = std::make_unique<ui::WidgetText>(45, 12, "Qty: 0");
	lumInvQuantityText->SetCharacterSize(14);
	lumInvQuantityText->SetTextColor(sf::Color::Yellow);
	lumInvQuantityText->SetStyle(sf::Text::Bold);
	m_lumInvQuantityText = lumInvQuantityText.get();
	m_lumInvContainer->AddWidget(std::move(lumInvQuantityText));

	// Lumirite volume text
	auto lumInvVolumeText = std::make_unique<ui::WidgetText>(150, 12, "Volume: 0");
	lumInvVolumeText->SetCharacterSize(14);
	lumInvVolumeText->SetTextColor(sf::Color::Cyan);
	lumInvVolumeText->SetStyle(sf::Text::Bold);
	m_lumInvVolumeText = lumInvVolumeText.get();
	m_lumInvContainer->AddWidget(std::move(lumInvVolumeText));

	// === Nanochip Inventory Button Container ===
	auto nanInvContainer = std::make_unique<ui::WidgetContainer>(0, 0, 530, 45);
	nanInvContainer->SetLayout(ui::LayoutType::Native);
	m_nanInvContainer = nanInvContainer.get();
	m_inventoryProductsContainer->AddWidget(std::move(nanInvContainer));

	// Nanochip inventory button
	auto nanInvButton = std::make_unique<ui::WidgetButton>(0, 0, 530, 45);
	nanInvButton->LoadImage("ButtonMain2.png");
	nanInvButton->SetOnClickCallback([this]() {
		// TODO: Handle Nanochip inventory action
		});
	m_nanInvButton = nanInvButton.get();
	m_nanInvContainer->AddWidget(std::move(nanInvButton));

	// Nanochip icon
	auto nanInvIcon = std::make_unique<ui::WidgetImage>(5, 7, 30, 30, "IconMaterialNano.png");
	m_nanInvIcon = nanInvIcon.get();
	m_nanInvContainer->AddWidget(std::move(nanInvIcon));

	// Nanochip quantity text
	auto nanInvQuantityText = std::make_unique<ui::WidgetText>(45, 12, "Qty: 0");
	nanInvQuantityText->SetCharacterSize(14);
	nanInvQuantityText->SetTextColor(sf::Color::Yellow);
	nanInvQuantityText->SetStyle(sf::Text::Bold);
	m_nanInvQuantityText = nanInvQuantityText.get();
	m_nanInvContainer->AddWidget(std::move(nanInvQuantityText));

	// Nanochip volume text
	auto nanInvVolumeText = std::make_unique<ui::WidgetText>(150, 12, "Volume: 0");
	nanInvVolumeText->SetCharacterSize(14);
	nanInvVolumeText->SetTextColor(sf::Color::Cyan);
	nanInvVolumeText->SetStyle(sf::Text::Bold);
	m_nanInvVolumeText = nanInvVolumeText.get();
	m_nanInvContainer->AddWidget(std::move(nanInvVolumeText));

	// Add volume text
	auto volumeText = std::make_unique<ui::WidgetText>(270, 330, "Volume");
	volumeText->SetCharacterSize(18);
	volumeText->SetStyle(sf::Text::Bold);
	volumeText->SetAlignment(ui::WidgetText::Alignment::Center);
	volumeText->SetTextColor(sf::Color::White);
	m_volumeText = volumeText.get();
	m_inventoryContainer->AddWidget(std::move(volumeText));

	// Create Volume Progress Bar (green theme) with "VOL" suffix
	auto volumeProgressBar = std::make_unique<ui::WidgetProgressBar>(50, 355, 450, 25, " VOL");
	volumeProgressBar->SetForegroundColor(sf::Color(20, 220, 20)); // Green
	volumeProgressBar->SetBackgroundColor(sf::Color(64, 64, 64));   // Dark gray
	volumeProgressBar->SetBorderColor(sf::Color::White);
	volumeProgressBar->SetBorderThickness(2.0f);
	volumeProgressBar->SetProgress(0.30f);  // 30% volume
	volumeProgressBar->SetShowPercentage(true);
	volumeProgressBar->SetTextSize(12); // Smaller text

	// Set font if available
	if (m_digitalFont.getInfo().family != "")
	{
		volumeProgressBar->SetFont(m_digitalFont);
	}
	m_volumeProgressBar = volumeProgressBar.get();
	m_inventoryContainer->AddWidget(std::move(volumeProgressBar));
}

/// @brief Initialize the inventory sort selector with 2 toggle buttons
/// @details Creates a horizontal container with Volume and Quantity buttons
///          that switch between different inventory sorting modes. Only one button
///          can be selected at a time, with the selected button showing a different background.
///          Position: Above the inventory container (bottom left area)
void ApplicationUI::UI_InitializeInventorySortSelector()
{
	// Create inventory sort selector container (small horizontal container above inventory)
	auto inventorySortSelectorContainer = std::make_unique<ui::WidgetContainer>(30, 550, 370, 50);
	inventorySortSelectorContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
	m_inventorySortSelectorContainer = inventorySortSelectorContainer.get(); // Store raw pointer for later access
	m_rootContainer->AddWidget(std::move(inventorySortSelectorContainer));

	// === Volume Sort Button ===
	auto volumeSortButton = std::make_unique<ui::WidgetButton>(0, 0, 270, 50);
	volumeSortButton->LoadImage("ButtonMain2.png"); // Default button background
	volumeSortButton->SetText("VOLUME");
	volumeSortButton->SetTextColor(sf::Color::White);
	volumeSortButton->SetOnClickCallback([this]() {
		SelectInventorySort(InventorySortType::Volume); // Select Volume sort
		});
	m_volumeSortButton = volumeSortButton.get();
	m_inventorySortSelectorContainer->AddWidget(std::move(volumeSortButton));

	// === Quantity Sort Button ===
	auto quantitySortButton = std::make_unique<ui::WidgetButton>(271, 0, 270, 50);
	quantitySortButton->LoadImage("ButtonMain2.png"); // Default button background
	quantitySortButton->SetText("QUANTITY");
	quantitySortButton->SetTextColor(sf::Color::White);
	quantitySortButton->SetOnClickCallback([this]() {
		SelectInventorySort(InventorySortType::Quantity); // Select Quantity sort
		});
	m_quantitySortButton = quantitySortButton.get();
	m_inventorySortSelectorContainer->AddWidget(std::move(quantitySortButton));

	// Set initial selection to Volume (default)
	SelectInventorySort(InventorySortType::Volume);
}

/// @brief Initialize the product info container in the bottom right corner
/// @details Creates a container for displaying product information such as:
///          - Current product details when selected
///          - Trading statistics and market trends
///          - Product status and notifications
///          Position: bottom right corner of the screen
void ApplicationUI::UI_InitializeProductInfoContainer()
{
 // Create product info container in bottom right corner (550x450 pixels)
 auto productInfoContainer = std::make_unique<ui::WidgetContainer>(1340, 600, 550, 450);
 productInfoContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
 m_productInfoContainer = productInfoContainer.get(); // Store raw pointer for later access
 m_rootContainer->AddWidget(std::move(productInfoContainer));

 // Add background for product info container
 auto productInfoBackground = std::make_unique<ui::WidgetImage>(0, 0, 550, 450, "BgInventory.png");
 m_productInfoContainer->AddWidget(std::move(productInfoBackground));

 // Add product info title text
 auto productInfoTitle = std::make_unique<ui::WidgetText>(275, 40, "PRODUCT INFO");
 productInfoTitle->SetCharacterSize(24);
 productInfoTitle->SetStyle(sf::Text::Bold);
 productInfoTitle->SetAlignment(ui::WidgetText::Alignment::Center);
 productInfoTitle->SetTextColor(sf::Color::White);
 m_productInfoContainer->AddWidget(std::move(productInfoTitle));

 // Add product info image
 auto productInfoImage = std::make_unique<ui::WidgetImage>(300, 100, 200, 200, "IconMaterialLumi.png");
 m_productInfoImage = productInfoImage.get();
 m_productInfoContainer->AddWidget(std::move(productInfoImage));

 // Add product name text
 auto productNameText = std::make_unique<ui::WidgetText>(50, 80, "Name ");
 productNameText->SetCharacterSize(18);
 productNameText->SetTextColor(sf::Color::White);
 productNameText->SetStyle(sf::Text::Bold);
 m_productNameText = productNameText.get();
 m_productInfoContainer->AddWidget(std::move(productNameText));

 // Add product volume text
 auto productVolumeText = std::make_unique<ui::WidgetText>(50, 120, "Volume init");
 productVolumeText->SetCharacterSize(18);
 productVolumeText->SetTextColor(sf::Color::White);
 productVolumeText->SetStyle(sf::Text::Bold);
 m_productVolumeText = productVolumeText.get();
 m_productInfoContainer->AddWidget(std::move(productVolumeText));

 // Add product rarity text
 auto productRarityText = std::make_unique<ui::WidgetText>(50, 160, "Rarity");
 productRarityText->SetCharacterSize(18);
 productRarityText->SetTextColor(sf::Color::White);
 productRarityText->SetStyle(sf::Text::Bold);
 m_productRarityText = productRarityText.get();
 m_productInfoContainer->AddWidget(std::move(productRarityText));

   // Add product info text
 auto productInfoText = std::make_unique<ui::WidgetText>(50, 200, "PI: None ");
 productInfoText->SetCharacterSize(18);
 productInfoText->SetTextColor(sf::Color::White);
 productInfoText->SetStyle(sf::Text::Bold);
 m_productInfoText = productInfoText.get();
 m_productInfoContainer->AddWidget(std::move(productInfoText));

 // Initially hide product info container until a monitor is selected
 m_productInfoContainer->SetVisible(false);
}

///@brief Initialize the company info container in the bottom right corner
///@details Creates a container for displaying company information such as:
//         - Current company details when selected
//         - Company statistics and information
//         - Company status and notifications
//         Position: bottom right corner of the screen (same as product info)
void ApplicationUI::UI_InitializeCompanyInfoContainer()
{
	// Create company info container in bottom right corner (550x450 pixels)
	auto productInfoContainer = std::make_unique<ui::WidgetContainer>(1340, 600, 550, 450);
	productInfoContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
	m_companyInfoContainer = productInfoContainer.get(); // Store raw pointer for later access
	m_rootContainer->AddWidget(std::move(productInfoContainer));

	// Add background for company info container
	auto companyInfoBackground = std::make_unique<ui::WidgetImage>(0, 0, 550, 450, "BgInventory.png");
	m_companyInfoContainer->AddWidget(std::move(companyInfoBackground));

	// Add company info title text
	auto companyInfoTitle = std::make_unique<ui::WidgetText>(275, 40, "COMPANY INFO");
	companyInfoTitle->SetCharacterSize(24);
	companyInfoTitle->SetStyle(sf::Text::Bold);
	companyInfoTitle->SetAlignment(ui::WidgetText::Alignment::Center);
	companyInfoTitle->SetTextColor(sf::Color::White);
	m_companyInfoContainer->AddWidget(std::move(companyInfoTitle));

	//Add company name text
	auto companyNameText = std::make_unique<ui::WidgetText>(50, 80, "Company: None ");
	companyNameText->SetCharacterSize(18);
	companyNameText->SetTextColor(sf::Color::White);
	companyNameText->SetStyle(sf::Text::Bold);
	m_companyNameText = companyNameText.get();
	m_companyInfoContainer->AddWidget(std::move(companyNameText));

	// Add company detailed info text
	auto companyInfoText = std::make_unique<ui::WidgetText>(50, 110, "Company info...");
	companyInfoText->SetCharacterSize(14);
	companyInfoText->SetTextColor(sf::Color::White);
	companyInfoText->SetStyle(sf::Text::Bold);
	m_companyInfoText = companyInfoText.get();
	m_companyInfoContainer->AddWidget(std::move(companyInfoText));

	// Add company logo image
	auto companyLogo = std::make_unique<ui::WidgetImage>(300, 120, 200, 150, "Lumacore.png");
	m_companyLogo = companyLogo.get();
	m_companyInfoContainer->AddWidget(std::move(companyLogo));

  // // Initially hide company info container until a monitor is selected
	m_companyInfoContainer->SetVisible(false);
}

/// @brief Initialize the vendor info container in the bottom right corner
/// @details Creates a container for displaying vendor information such as:
///          - Current vendor details when selected
///          - Vendor statistics and information
///          - Vendor status and notifications
///          Position: bottom right corner of the screen (same as company info)
void ApplicationUI::UI_InitializeVendorInfoContainer()
{
	  // Create vendor info container in bottom right corner (550x450 pixels)
	  auto vendorInfoContainer = std::make_unique<ui::WidgetContainer>(1340, 600, 550, 450);
	  vendorInfoContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
	  m_vendorInfoContainer = vendorInfoContainer.get(); // Store raw pointer for later access
	  m_rootContainer->AddWidget(std::move(vendorInfoContainer));

	  // Add background for vendor info container
	  auto vendorInfoBackground = std::make_unique<ui::WidgetImage>(0, 0, 550, 450, "BgInventory.png");
	  m_vendorInfoContainer->AddWidget(std::move(vendorInfoBackground));

	  // Add vendor info title text
	  auto vendorInfoTitle = std::make_unique<ui::WidgetText>(275, 40, "VENDOR INFO");
	  vendorInfoTitle->SetCharacterSize(24);
	  vendorInfoTitle->SetStyle(sf::Text::Bold);
	  vendorInfoTitle->SetAlignment(ui::WidgetText::Alignment::Center);
	  vendorInfoTitle->SetTextColor(sf::Color::White);
	  m_vendorInfoContainer->AddWidget(std::move(vendorInfoTitle));

	  // Add vendor character image
	  auto vendorImage = std::make_unique<ui::WidgetImage>(300, 80, 200, 300, "CharacterNano.png");
	  m_vendorImage = vendorImage.get();
	  m_vendorInfoContainer->AddWidget(std::move(vendorImage));

	  // Add vendor name text widget
	  auto vendorNameText = std::make_unique<ui::WidgetText>(50, 80, "Vendor Name");
	  vendorNameText->SetCharacterSize(18);
	  vendorNameText->SetTextColor(sf::Color::White);
	  vendorNameText->SetStyle(sf::Text::Bold);
	  m_vendorNameText = vendorNameText.get();
	  m_vendorInfoContainer->AddWidget(std::move(vendorNameText));

	  // Add vendor role text
	  auto vendorRoleText = std::make_unique<ui::WidgetText>(50, 120, "Role: ");
	  vendorRoleText->SetCharacterSize(18);
	  vendorRoleText->SetTextColor(sf::Color::White);
	  vendorRoleText->SetStyle(sf::Text::Bold);
	  m_vendorRoleText = vendorRoleText.get();
	  m_vendorInfoContainer->AddWidget(std::move(vendorRoleText));

	  // Add vendor profile text widget (multi-line description)
	  auto vendorProfileText = std::make_unique<ui::WidgetText>(50, 170, "Vendor Profile");
	  vendorProfileText->SetCharacterSize(18);
	  vendorProfileText->SetTextColor(sf::Color::White);
	  m_vendorProfileText = vendorProfileText.get();
	  m_vendorInfoContainer->AddWidget(std::move(vendorProfileText));

	  // Initially hide vendor info container until a monitor is selected
	  m_vendorInfoContainer->SetVisible(false);
}

/// @brief Initialize the info panel selector with 3 toggle buttons
/// @details Creates a horizontal container with Product, Company, and Vendor buttons
///          that switch between the corresponding info containers. Only one button
///          can be selected at a time, with the selected button showing a different background.
///          Position: Above the info containers (bottom right area)
void ApplicationUI::UI_InitializeInfoPanelSelector()
{
	// Create info panel selector container (small horizontal container above info panels)
	auto infoPanelSelectorContainer = std::make_unique<ui::WidgetContainer>(1340, 550, 550, 50);
	infoPanelSelectorContainer->SetLayout(ui::LayoutType::Native); // Manual positioning for precise control
	m_infoPanelSelectorContainer = infoPanelSelectorContainer.get(); // Store raw pointer for later access
	m_rootContainer->AddWidget(std::move(infoPanelSelectorContainer));

	// === Product Info Selector Button ===
	auto productInfoSelectorButton = std::make_unique<ui::WidgetButton>(0, 0, 180, 50);
	productInfoSelectorButton->LoadImage("ButtonMain2.png"); // Default button background
	productInfoSelectorButton->SetText("PRODUCT");
	productInfoSelectorButton->SetTextColor(sf::Color::White);
	productInfoSelectorButton->SetOnClickCallback([this]() {
		SelectInfoPanel(0); // Select Product info panel
		});
	m_productInfoSelectorButton = productInfoSelectorButton.get();
	m_infoPanelSelectorContainer->AddWidget(std::move(productInfoSelectorButton));

	// === Company Info Selector Button ===
	auto companyInfoSelectorButton = std::make_unique<ui::WidgetButton>(185, 0, 180, 50);
	companyInfoSelectorButton->LoadImage("ButtonMain2.png"); // Default button background
	companyInfoSelectorButton->SetText("COMPANY");
	companyInfoSelectorButton->SetTextColor(sf::Color::White);
	companyInfoSelectorButton->SetOnClickCallback([this]() {
		SelectInfoPanel(1); // Select Company info panel
		});
	m_companyInfoSelectorButton = companyInfoSelectorButton.get();
	m_infoPanelSelectorContainer->AddWidget(std::move(companyInfoSelectorButton));

	// === Vendor Info Selector Button ===
	auto vendorInfoSelectorButton = std::make_unique<ui::WidgetButton>(370, 0, 180, 50);
	vendorInfoSelectorButton->LoadImage("ButtonMain2.png"); // Default button background
	vendorInfoSelectorButton->SetText("VENDOR");
	vendorInfoSelectorButton->SetTextColor(sf::Color::White);
	vendorInfoSelectorButton->SetOnClickCallback([this]() {
		SelectInfoPanel(2); // Select Vendor info panel
		});
	m_vendorInfoSelectorButton = vendorInfoSelectorButton.get();
	m_infoPanelSelectorContainer->AddWidget(std::move(vendorInfoSelectorButton));

	// Set initial selection to Product (index 0)
	SelectInfoPanel(0);
	
	// Initially hide info panel selector until a monitor is selected
	m_infoPanelSelectorContainer->SetVisible(false);
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

/// @brief Initialize the logo image in the top left corner
/// @details Creates a logo image widget positioned in the top left corner of the screen.
///          The logo provides branding and visual identity for the application.
///          Position: (20, 20) - top left corner with small margin
void ApplicationUI::UI_InitializeLogo()
{
	// Create logo image widget in top left corner
	auto logoImage = std::make_unique<ui::WidgetImage>(20, -20, 300, 300, "Logo.png");

	// Store pointer for later access
	m_logoImage = logoImage.get();

	// Add to root container
	m_rootContainer->AddWidget(std::move(logoImage));
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
	return; // DEBUG DISABLED - comment out this line to enable debug borders
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

	// Trade container debug (yellow border)
	if (m_tradeContainer)
		m_tradeContainer->EnableDebugDraw(true, sf::Color(255, 255, 0, 255)); // Yellow opaque

	// Inventory container debug (magenta border)
	if (m_inventoryContainer)
		m_inventoryContainer->EnableDebugDraw(true, sf::Color(255, 0, 255, 255)); // Magenta opaque

	// Product Info container debug (cyan border)
	if (m_productInfoContainer)
		m_productInfoContainer->EnableDebugDraw(true, sf::Color(0, 255, 255, 255)); // Cyan opaque

	// Company Info container debug (yellow border)
	if (m_companyInfoContainer)
		m_companyInfoContainer->EnableDebugDraw(true, sf::Color(255, 255, 0, 128)); // Yellow semi-transparent

	// Vendor Info container debug (white border)
	if (m_vendorInfoContainer)
		m_vendorInfoContainer->EnableDebugDraw(true, sf::Color(255, 255, 255, 128)); // White semi-transparent
}

// Initialize progress bars for cycle, etc.
void ApplicationUI::UI_InitializeProgressBars()
{
	// Create Cycle Progress Bar (orange theme) with "CYCLE" suffix - positioned at top center (2x size)
	auto cycleProgressBar = std::make_unique<ui::WidgetProgressBar>(630, 150, 720, 25, " CYCLE");
	cycleProgressBar->SetForegroundColor(sf::Color(255, 165, 0));  // Orange
	cycleProgressBar->SetBackgroundColor(sf::Color(64, 64, 64));   // Dark gray
	cycleProgressBar->SetBorderColor(sf::Color::White);
	cycleProgressBar->SetBorderThickness(2.0f);
	cycleProgressBar->SetProgress(0.60f);  // 60% cycle progress
	cycleProgressBar->SetShowPercentage(true);
	cycleProgressBar->SetTextSize(12); // Smaller text

	// Set font if available
	if (m_digitalFont.getInfo().family != "")
	{
		cycleProgressBar->SetFont(m_digitalFont);
	}  m_cycleProgressBar = cycleProgressBar.get();
	m_rootContainer->AddWidget(std::move(cycleProgressBar));
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

/// @brief Update rolling text animations - moves text continuously from right to left
/// @param deltaTime Time elapsed since last frame in seconds
/// @details This function animates the rolling news ticker by:
///          - Moving both rolling texts to the left at constant speed
///          - Resetting position when text moves completely off-screen
///          - Using different starting positions for staggered effect
void ApplicationUI::UpdateApplicationUI(sf::Time deltaTime)
{

	// Safety check for rolling text widgets
	if (!m_rollingText1 || !m_rollingText2)
		return;

	// Update rolling text 1 position
	m_rollingText1Position -= m_rollingSpeed * deltaTime.asSeconds();

	// Reset position when text moves completely off-screen to the left
	if (m_rollingText1Position < -1920.0f) // Assume text width is ~500px
	{
		m_rollingText1Position = 1920.0f; // Reset to right side of screen
		LoadingNewText(true);
		// Get new news content when rolling text 1 resets
		if (m_application && m_application->m_stockMarket)
		{
			News* nextNews = m_application->m_stockMarket->GetNextNews();
			if (nextNews)
			{
				m_rollingText1->SetText(nextNews->m_newsContent);
			}
		}
	}
	// Update rolling text 2 position (offset by half screen for staggered effect)
	m_rollingText2Position -= m_rollingSpeed * deltaTime.asSeconds();

	// Reset position when text moves completely off-screen to the left
	if (m_rollingText2Position < -1920.0f) // Assume text width is ~500px
	{
		m_rollingText2Position = 1920.0f; // Reset with offset for staggered effect
		DebugLog("PutBack Rolling Text 2 to position: " + std::to_string(m_rollingText2Position));
		LoadingNewText(false);
		// Get new news content when rolling text 2 resets
		if (m_application && m_application->m_stockMarket)
		{
			News* nextNews = m_application->m_stockMarket->GetNextNews();
			if (nextNews)
			{
				m_rollingText2->SetText(nextNews->m_newsContent);
			}
		}
	}

	// Apply new positions to the widgets
	m_rollingText1->SetPosition(m_rollingText1Position, 17.0f);
	//DebugLog("Rolling Text 1 position: " + std::to_string(m_rollingText1Position));

	m_rollingText2->SetPosition(m_rollingText2Position, 17.0f);
	//DebugLog("Rolling Text 2 position: " + std::to_string(m_rollingText2Position));


}

/// @brief Update cycle progress bar with remaining time countdown
/// Updates the cycle progress bar to show remaining time in the current market cycle
/// Displays countdown text with 1 decimal place precision
void ApplicationUI::UpdateCycleProgressBar()
{
	// Check if we have the necessary components
	if (!m_application || !m_application->m_stockMarket || !m_cycleProgressBar)
	{
		return;
	}

	// Get cycle timing information from stock market
	float currentCycleTime = m_application->m_stockMarket->m_currentCycleTime;
	float totalCycleTime = 5.0f; // s_stockCycleTime is 5.0f seconds

	// Calculate remaining time (countdown)
	float remainingTime = totalCycleTime - currentCycleTime;

	// Ensure remaining time is not negative (safety check)
	remainingTime = std::max(0.0f, remainingTime);

	// Calculate progress as ratio of elapsed time (0.0 to 1.0)
	float progressRatio = (totalCycleTime > 0.0f) ? (currentCycleTime / totalCycleTime) : 0.0f;

	// Ensure progress ratio stays within bounds
	progressRatio = std::max(0.0f, std::min(1.0f, progressRatio));

	// Set the progress bar value
	m_cycleProgressBar->SetProgress(progressRatio);

	// Create countdown text with 1 decimal place
	std::ostringstream countdownText;
	countdownText << std::fixed << std::setprecision(1) << remainingTime << "s";

	// Set the custom countdown text
	m_cycleProgressBar->SetCustomText(countdownText.str());
}

/// @brief Update all inventory buttons with current quantity and volume values
/// Updates the 5 inventory product buttons to show current player inventory data
void ApplicationUI::UpdateInventoryButtons()
{
	// Check if we have the necessary components
	if (!m_application || !m_application->GetPlayerInventory() || !m_application->m_stockMarket)
	{
		return;
	}

	// Get player inventory reference
	Inventory* inventory = m_application->GetPlayerInventory();

	// Product IDs corresponding to each inventory button
	std::string productIds[5] = { "TRI", "NFX", "ZER", "LUM", "NAN" };

	// Update Tritanium (index 0)
	if (m_triInvQuantityText && m_triInvVolumeText)
	{
		uint32_t quantity = inventory->GetProductQuantity(productIds[0]);
		
		// Get product data for volume calculation
		StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[0]);
		float totalVolume = 0.0f;
		if (product)
		{
			totalVolume = quantity * product->m_volume;
		}

		// Update text widgets
		m_triInvQuantityText->SetText("Qty: " + std::to_string(quantity));
		
		std::ostringstream volumeStream;
		volumeStream << "Volume: " << std::fixed << std::setprecision(1) << totalVolume;
		m_triInvVolumeText->SetText(volumeStream.str());
	}

	// Update Neuroflux (index 1)
	if (m_nfxInvQuantityText && m_nfxInvVolumeText)
	{
		uint32_t quantity = inventory->GetProductQuantity(productIds[1]);
		
		StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[1]);
		float totalVolume = 0.0f;
		if (product)
		{
			totalVolume = quantity * product->m_volume;
		}

		m_nfxInvQuantityText->SetText("Qty: " + std::to_string(quantity));
		
		std::ostringstream volumeStream;
		volumeStream << "Volume: " << std::fixed << std::setprecision(1) << totalVolume;
		m_nfxInvVolumeText->SetText(volumeStream.str());
	}

	// Update Zeromass (index 2)
	if (m_zerInvQuantityText && m_zerInvVolumeText)
	{
		uint32_t quantity = inventory->GetProductQuantity(productIds[2]);
		
		StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[2]);
		float totalVolume = 0.0f;
		if (product)
		{
			totalVolume = quantity * product->m_volume;
		}

		m_zerInvQuantityText->SetText("Qty: " + std::to_string(quantity));
		
		std::ostringstream volumeStream;
		volumeStream << "Volume: " << std::fixed << std::setprecision(1) << totalVolume;
		m_zerInvVolumeText->SetText(volumeStream.str());
	}

	// Update Lumirite (index 3)
	if (m_lumInvQuantityText && m_lumInvVolumeText)
	{
		uint32_t quantity = inventory->GetProductQuantity(productIds[3]);
		
		StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[3]);
		float totalVolume = 0.0f;
		if (product)
		{
			totalVolume = quantity * product->m_volume;
		}

		m_lumInvQuantityText->SetText("Qty: " + std::to_string(quantity));
		
		std::ostringstream volumeStream;
		volumeStream << "Volume: " << std::fixed << std::setprecision(1) << totalVolume;
		m_lumInvVolumeText->SetText(volumeStream.str());
	}

	// Update Nanochip (index 4)
	if (m_nanInvQuantityText && m_nanInvVolumeText)
	{
		uint32_t quantity = inventory->GetProductQuantity(productIds[4]);
		
		StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[4]);
		float totalVolume = 0.0f;
		if (product)
		{
			totalVolume = quantity * product->m_volume;
		}

		m_nanInvQuantityText->SetText("Qty: " + std::to_string(quantity));
		
		std::ostringstream volumeStream;
		volumeStream << "Volume: " << std::fixed << std::setprecision(1) << totalVolume;
		m_nanInvVolumeText->SetText(volumeStream.str());
	}
}

/// @brief Select a monitor button and apply purple highlighting
/// @param monitorIndex Index of the monitor button to select (0-4)
void ApplicationUI::SelectMonitor(int monitorIndex) {
	if (monitorIndex < 0 || monitorIndex >= 5) {
		return;
	}

	// If clicking the same monitor that's already selected, do nothing
	if (m_selectedMonitorIndex == monitorIndex) {
		return;
	}

	// Cancel any previous selection
	CancelSelection();

	// Set the new selection
	m_selectedMonitorIndex = monitorIndex;

	// Show purple highlight overlay for selected monitor
	if (m_monitorHighlights[monitorIndex]) {
		m_monitorHighlights[monitorIndex]->SetVisible(true);
	}

	// Show trade container and info panel selector when a monitor is selected
	if (m_tradeContainer) {
		m_tradeContainer->SetVisible(true);
	}
	if (m_infoPanelSelectorContainer) {
		m_infoPanelSelectorContainer->SetVisible(true);
	}

	// Show the currently selected info container based on selector state
	SelectInfoPanel(m_selectedInfoPanel);

	// Set the current product ID based on monitor index and update info containers
	if (m_application && m_application->m_stockMarket) {
		// Product IDs corresponding to each monitor (same as UpdateProductDisplays)
		std::string productIds[5] = { "TRI", "NFX", "ZER", "LUM", "NAN" };

		// Set current product ID
		m_application->m_stockMarket->SetCurrentProductID(productIds[monitorIndex]);

		// Get product data for info containers
		StockProduct* product = m_application->m_stockMarket->GetStockProductById(productIds[monitorIndex]);
		if (product) {
			// Update product info text with product description
			if (m_productInfoText) {
				m_productInfoText->SetText(product->m_productInfo);
			}

			// Update product volume text with volume value
			if (m_productVolumeText) {
				std::ostringstream volumeStream;
				volumeStream << "Volume: " << std::fixed << std::setprecision(1) << product->m_volume;
				m_productVolumeText->SetText(volumeStream.str());
			}

			// Update product rarity text with rarity level and color
			if (m_productRarityText) {
				std::string rarityText;
				sf::Color rarityColor;

				switch (product->m_itemRarity) {
				case RarityLevel::Common:
					rarityText = "Rarity: Common";
					rarityColor = sf::Color::White;
					break;
				case RarityLevel::Normal:
					rarityText = "Rarity: Normal";
					rarityColor = sf::Color::Cyan;
					break;
				case RarityLevel::Rare:
					rarityText = "Rarity: Rare";
					rarityColor = sf::Color::Green;
					break;
				default:
					rarityText = "Rarity: Unknown";
					rarityColor = sf::Color::White;
					break;
				}

				m_productRarityText->SetText(rarityText);
				m_productRarityText->SetTextColor(rarityColor);
			}

			// Update product info image based on selected product ID
			if (m_productInfoImage) {
				std::string productImageFile;
				switch (monitorIndex) {
				case 0: productImageFile = "IconMaterialTritanium.png"; break;  // TRI
				case 1: productImageFile = "IconMaterialNeuro.png"; break;      // NFX  
				case 2: productImageFile = "IconMaterialZeromass.png"; break;   // ZER
				case 3: productImageFile = "IconMaterialLumi.png"; break;       // LUM
				case 4: productImageFile = "IconMaterialNano.png"; break;       // NAN
				default: productImageFile = "IconMaterialNano.png"; break;      // Fallback
				}
				m_productInfoImage->LoadImage(productImageFile);
			}

			// Update vendor character image based on selected product ID
			if (m_vendorImage) {
				std::string vendorImageFile;
				switch (monitorIndex) {
				case 0: vendorImageFile = "CharacterTriton.png"; break;    // TRI
				case 1: vendorImageFile = "CharacterFlux.png"; break;      // NFX
				case 2: vendorImageFile = "CharacterZeromass.png"; break;  // ZER
				case 3: vendorImageFile = "CharacterLuma.png"; break;      // LUM
				case 4: vendorImageFile = "CharacterNano.png"; break;      // NAN
				default: vendorImageFile = "CharacterNano.png"; break;     // Fallback
				}
				m_vendorImage->LoadImage(vendorImageFile);
			}

			// Update vendor product name text with selected product name
			if (m_productNameText) {
				m_productNameText->SetText(product->m_name);
			}

			// Update company name text with company name from vendor data
			if (m_companyNameText) {
				StockVendor* vendor = m_application->m_stockMarket->GetStockVendorByProductId(productIds[monitorIndex]);
				if (vendor) {
					m_companyNameText->SetText( vendor->m_company);
				} else {
					m_companyNameText->SetText("Unknown");
				}
			}

			// Update company detailed info text with company info from vendor data
			if (m_companyInfoText) {
				StockVendor* vendor = m_application->m_stockMarket->GetStockVendorByProductId(productIds[monitorIndex]);
				if (vendor) {
					m_companyInfoText->SetText(vendor->m_companyInfo);
				} else {
					m_companyInfoText->SetText("No company\ninformation\navailable.");
				}
			}

			// Update company logo based on selected product ID
			if (m_companyLogo) {
				std::string companyLogoFile;
				switch (monitorIndex) {
					case 0: companyLogoFile = "TritonDynamics.png"; break;     // TRI
					case 1: companyLogoFile = "FluxNeurals.png"; break;        // NFX
					case 2: companyLogoFile = "ZeromassLabs.png"; break;       // ZER
					case 3: companyLogoFile = "Lumacore.png"; break;           // LUM
					case 4: companyLogoFile = "NanodyneIndustries.png"; break; // NAN
					default: companyLogoFile = "Lumacore.png"; break;          // Fallback
				}
				m_companyLogo->LoadImage(companyLogoFile);
			}

			// Update role text with vendor role from vendor data
			if (m_vendorRoleText) {
				StockVendor* vendor = m_application->m_stockMarket->GetStockVendorByProductId(productIds[monitorIndex]);
				if (vendor) {
					m_vendorRoleText->SetText(vendor->m_role);
				} else {
					m_vendorRoleText->SetText("Unknown Role");
				}
			}

			// Update vendor name text with vendor name from vendor data
			if (m_vendorNameText) {
				StockVendor* vendor = m_application->m_stockMarket->GetStockVendorByProductId(productIds[monitorIndex]);
				if (vendor) {
					m_vendorNameText->SetText(vendor->m_name);
				} else {
					m_vendorNameText->SetText("Unknown Name");
				}
			}

			// Update vendor profile text with vendor profile from vendor data
			if (m_vendorProfileText) {
				StockVendor* vendor = m_application->m_stockMarket->GetStockVendorByProductId(productIds[monitorIndex]);
				if (vendor) {
					m_vendorProfileText->SetText(vendor->m_profile);
				} else {
					m_vendorProfileText->SetText("No profile\navailable.");
				}
			}
		}
	}
}

/// @brief Cancel the current monitor selection and hide purple highlight
void ApplicationUI::CancelSelection() {
	if (m_selectedMonitorIndex >= 0 && m_selectedMonitorIndex < 5) {
		// Hide the purple highlight overlay
		if (m_monitorHighlights[m_selectedMonitorIndex]) {
			m_monitorHighlights[m_selectedMonitorIndex]->SetVisible(false);
		}
	}

	// Hide trade container, info panel selector, and all info containers when selection is cancelled
	if (m_tradeContainer) {
		m_tradeContainer->SetVisible(false);
	}
	if (m_infoPanelSelectorContainer) {
		m_infoPanelSelectorContainer->SetVisible(false);
	}
	if (m_productInfoContainer) {
		m_productInfoContainer->SetVisible(false);
	}
	if (m_companyInfoContainer) {
		m_companyInfoContainer->SetVisible(false);
	}
	if (m_vendorInfoContainer) {
		m_vendorInfoContainer->SetVisible(false);
	}

	m_selectedMonitorIndex = -1;
}

/// @brief Select an info panel and update button states
/// @param panelIndex Index of the info panel to select (0=Product, 1=Company, 2=Vendor)
/// @details This function switches between the three info containers and updates the
///          selector button states. The selected button gets a different background
///          (BgInventory.png) and the corresponding info container is shown while others are hidden.
void ApplicationUI::SelectInfoPanel(int panelIndex) {
	if (panelIndex < 0 || panelIndex > 2) {
		return;
	}

	// Update selected panel index
	m_selectedInfoPanel = panelIndex;

	// Reset all selector buttons to default state (ButtonMain2.png)
	if (m_productInfoSelectorButton) {
		m_productInfoSelectorButton->LoadImage("ButtonMain2.png");
	}
	if (m_companyInfoSelectorButton) {
		m_companyInfoSelectorButton->LoadImage("ButtonMain2.png");
	}
	if (m_vendorInfoSelectorButton) {
		m_vendorInfoSelectorButton->LoadImage("ButtonMain2.png");
	}

	// Set selected button to pressed state (BgInventory.png)
	switch (panelIndex) {
	case 0: // Product
		if (m_productInfoSelectorButton) {
			m_productInfoSelectorButton->LoadImage("BgInventory.png");
		}
		break;
	case 1: // Company
		if (m_companyInfoSelectorButton) {
			m_companyInfoSelectorButton->LoadImage("BgInventory.png");
		}
		break;
	case 2: // Vendor
		if (m_vendorInfoSelectorButton) {
			m_vendorInfoSelectorButton->LoadImage("BgInventory.png");
		}
		break;
	}

	// Show/hide info containers based on selection
	// Only show containers if a monitor is currently selected
	bool monitorSelected = (m_selectedMonitorIndex >= 0);

	if (m_productInfoContainer) {
		m_productInfoContainer->SetVisible(monitorSelected && panelIndex == 0);
	}
	if (m_companyInfoContainer) {
		m_companyInfoContainer->SetVisible(monitorSelected && panelIndex == 1);
	}
	if (m_vendorInfoContainer) {
		m_vendorInfoContainer->SetVisible(monitorSelected && panelIndex == 2);
	}
}

/// @brief Select an inventory sort type and update button states
/// @param sortType The inventory sort type to select (Volume, Value, or Quantity)
/// @details This function switches between the three inventory sorting modes and updates the
///          selector button states. The selected button gets a different background
///          (BgInventory.png) and calls the inventory sorting function with the selected type.
void ApplicationUI::SelectInventorySort(InventorySortType sortType) {
	// Update selected sort type
	m_selectedSortType = sortType;

	// Reset all selector buttons to default state (ButtonMain2.png)
	if (m_volumeSortButton) {
		m_volumeSortButton->LoadImage("ButtonMain2.png");
	}
	if (m_quantitySortButton) {
		m_quantitySortButton->LoadImage("ButtonMain2.png");
	}

	// Set selected button to pressed state (BgInventory.png)
	switch (sortType) {
	case InventorySortType::Volume:
		if (m_volumeSortButton) {
			m_volumeSortButton->LoadImage("BgInventory.png");
		}
		break;
	case InventorySortType::Quantity:
		if (m_quantitySortButton) {
			m_quantitySortButton->LoadImage("BgInventory.png");
		}
		break;
	}

	// TODO: Call inventory sorting function with selected sort type
	// This will be implemented later when inventory system is ready
	// SortInventoryItems(sortType);
}

/// @brief Load initial news content into rolling text widgets
/// @details Called once during UI initialization to populate rolling text with real news
///          instead of placeholder text. Gets first two news items from stock market.
void ApplicationUI::LoadingNewText(bool first)
{
	// Safety check: ensure we have application and stock market references
	if (!m_application || !m_application->m_stockMarket)
		return;

	// Load first news item for rolling text 1
	if (m_rollingText1)
	{
		News* firstNews = m_application->m_stockMarket->GetNextNews();
		if (firstNews)
		{
			m_rollingText1->SetText(firstNews->m_newsContent);
		}
	}

	// Load second news item for rolling text 2  
	if (m_rollingText2)
	{
		News* secondNews = m_application->m_stockMarket->GetNextNews();
		if (secondNews)
		{
			m_rollingText2->SetText(secondNews->m_newsContent);
		}
	}
}

/// @brief Update current money display with latest player money amount
/// @details Updates the money text widget in trade container to show current player money.
///          Should be called whenever money changes (buy/sell transactions, etc.)
void ApplicationUI::UpdateCurrentMoneyDisplay()
{
	// Safety check: ensure we have valid application, inventory, and money text widget
	if (!m_application || !m_application->GetPlayerInventory() || !m_currentMoneyText)
		return;

	// Get current player money from inventory
	uint32_t currentMoney = m_application->GetPlayerInventory()->GetCurrentMoney();
	
	// Format money display with $ symbol and comma separators for readability
	std::string moneyText = "Money: $" + std::to_string(currentMoney);
	
	// Update the text widget
	m_currentMoneyText->SetText(moneyText);
}
