#include "pch.h"
#include "application.h" // Aggregates required widget headers

// Main UI initialization - coordinates all container and widget creation
void Application::InitializeUI()
{
	// Create containers and their widgets
	InitializeContainersUI();

	// Final layout update after full hierarchy established
	m_rootWidgetContainer->UpdateLayout();
}

// Creates all containers and calls their individual initialization functions
void Application::InitializeContainersUI()
{
	// Initialize root and all child containers
	UI_InitializeRootContainer();
	UI_InitializeMainMenuContainer();
	UI_InitializeSubMenuContainer();
	UI_InitializeGameTimeWidget();

	// Enable debug drawing for all containers
	UI_DebugContainers();
}

// Initialize root container and child containers (without widgets)
void Application::UI_InitializeRootContainer()
{
	// Root container - FULL SCREEN
	m_rootWidgetContainer = std::make_unique<ui::WidgetContainer>(0, 0, 1920, 1080);

	// ADD BACKGROUND FIRST (so it's at the bottom Z-order)
	auto imageWidget = std::make_unique<ui::WidgetImage>(0, 0, 1920, 1080, "BgInit.png");
	m_rootWidgetContainer->AddWidget(std::move(imageWidget));

	// Title text (above background, below containers)
	auto titleText = std::make_unique<ui::WidgetText>(960, 100, "Hyper Trade");
	titleText->SetCharacterSize(30);
	titleText->SetStyle(sf::Text::Bold);
	titleText->SetAlignment(ui::WidgetText::Alignment::Center);
	titleText->SetTextColor(sf::Color::White);
	m_rootWidgetContainer->AddWidget(std::move(titleText));

	// Main menu container - store pointer before moving (ON TOP)
	auto menuMainContainer = std::make_unique<ui::WidgetContainer>(0, 200, 1920, 300);  // Increased height to 300
	menuMainContainer->SetLayout(ui::LayoutType::Horizontal, 5);
	m_mainMenuContainer = menuMainContainer.get(); // Store raw pointer
	m_rootWidgetContainer->AddWidget(std::move(menuMainContainer));

	// Sub menu container - store pointer before moving (ON TOP)
	auto menuSubContainer = std::make_unique<ui::WidgetContainer>(50, 500, 500, 600);
	menuSubContainer->SetLayout(ui::LayoutType::Vertical, 50);
	m_subMenuContainer = menuSubContainer.get(); // Store raw pointer
	m_rootWidgetContainer->AddWidget(std::move(menuSubContainer));
}

// Initialize main menu container with its buttons
void Application::UI_InitializeMainMenuContainer()
{
	if (!m_mainMenuContainer) return;

	auto button1 = std::make_unique<ui::WidgetButton>(20, 0, 300, 250);
	button1->LoadImage("ButtonMain1.png");
	button1->SetOnClickCallback([](){ /* Button 1 click callback */ });
	m_mainMenuContainer->AddWidget(std::move(button1));

	auto button2 = std::make_unique<ui::WidgetButton>(500, 20, 300, 250);
	button2->LoadImage("ButtonMain2.png");
	button2->SetOnClickCallback([](){ /* Button 2 click callback */ });
	m_mainMenuContainer->AddWidget(std::move(button2));

	auto button3 = std::make_unique<ui::WidgetButton>(900, 40, 300, 250);
	button3->LoadImage("ButtonMain3.png");
	button3->SetOnClickCallback([](){ /* Button 3 click callback */ });
	m_mainMenuContainer->AddWidget(std::move(button3));
}

// Initialize sub menu container with its buttons
void Application::UI_InitializeSubMenuContainer()
{
	if (!m_subMenuContainer) return;

	auto subButton1 = std::make_unique<ui::WidgetButton>(10, 10, 300, 100);
	subButton1->LoadImage("ButtonMain1.png");
	subButton1->SetOnClickCallback([](){ /* Sub Button 1 click */ });
	m_subMenuContainer->AddWidget(std::move(subButton1));

	auto subButton2 = std::make_unique<ui::WidgetButton>(30, 200, 200, 100);
	subButton2->LoadImage("ButtonMain2.png");
	subButton2->SetOnClickCallback([](){ /* Sub Button 2 click */ });
	m_subMenuContainer->AddWidget(std::move(subButton2));

	auto subButton3 = std::make_unique<ui::WidgetButton>(50, 300, 200, 100);
	subButton3->LoadImage("ButtonMain3.png");
	subButton3->SetOnClickCallback([](){ /* Sub Button 3 click */ });
	m_subMenuContainer->AddWidget(std::move(subButton3));
}

// Initialize game time display widget
void Application::UI_InitializeGameTimeWidget()
{
	auto gameTimeText = std::make_unique<ui::WidgetText>(1800, 100, "GametimeInitText", "FontDigitalNumbers.ttf");
	gameTimeText->SetCharacterSize(20);
	gameTimeText->SetStyle(sf::Text::Bold);
	gameTimeText->SetAlignment(ui::WidgetText::Alignment::Center);
	gameTimeText->SetTextColor(sf::Color::White);
	m_gameTimeText = gameTimeText.get();
	m_rootWidgetContainer->AddWidget(std::move(gameTimeText));
}

// Enable/disable debug drawing for all containers and widgets
void Application::UI_DebugContainers()
{
	// Root container debug (red border)
	if (m_rootWidgetContainer)
	{
		m_rootWidgetContainer->EnableDebugDraw(true, sf::Color(255, 0, 0, 255)); // Red opaque
	}

	// Main menu container debug (green border)
	if (m_mainMenuContainer)
	{
		m_mainMenuContainer->EnableDebugDraw(true, sf::Color(0, 255, 0, 128)); // Green semi-transparent

		// Enable debug for buttons in main menu
		for (size_t i = 0; i < m_mainMenuContainer->GetWidgetCount(); ++i)
		{
			// Note: This would require a way to get child widgets by index
			// For now, buttons debug is handled during creation if needed
		}
	}

	// Sub menu container debug (blue border)
	if (m_subMenuContainer)
	{
		m_subMenuContainer->EnableDebugDraw(true, sf::Color(0, 0, 255, 128)); // Blue semi-transparent
	}
}
