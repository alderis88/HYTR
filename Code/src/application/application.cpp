#include "pch.h"
#include "../framework/window.h"
#include "../framework/WidgetImage.h"
#include "../framework/WidgetButton.h"
#include "../framework/WidgetText.h"
#include "inventory.h"
#include "application.h"
#include "utilTools.h"
#include "stockMarket.h"

// Static member definitions
std::string Application::s_dataPath;
std::string Application::s_assetsPath;

Application::Application() = default;
Application::~Application() = default;

float Application::s_totalGameTime = 0.0f; // Initialize total game time to 0
bool Application::s_pauseGame = false;
float Application::s_stockGameSpeed = 1.0f;

std::mt19937& Application::GetRandomGenerator()
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	return generator;
}

void Application::Initialize()
{
	SetVideoSettings();

	SetDataPath("../../data/");
	SetAssetsPath("../../assets/");

	StockMarketSetup();
	//InventorySetup();

	if (m_mainWindow = std::make_unique<ui::Window>())
	{
		// here you can create your layout
		InitializeUI();
	}
}


void Application::SetVideoSettings()
{
	m_renderContext = std::make_unique<RenderContext>(sf::VideoMode(1920, 1080), "Recruitment task");
	m_renderContext->setFramerateLimit(30);
}

void Application::StockMarketSetup()
{
	m_stockMarket = std::make_unique<StockMarket>();
	m_stockMarket->InitializeStockMarket();
}

void Application::InventorySetup()
{
	m_playerInventory = std::make_unique<Inventory>();
	m_playerInventory->InventoryInitialize();
}

void Application::InitializeUI()
{

	// Create background image that fills the entire window (1920x1080) - now with filename in constructor
	auto imageWidget = std::make_unique<ui::WidgetImage>(0, 0, 1920, 1080, "BgInit.png");
	m_mainWindow->AddWidget(std::move(imageWidget));

	// Create title text "Hyper Trade" at the top (10 pixels from top)
	auto titleText = std::make_unique<ui::WidgetText>(960, 100, "Hyper Trade");
	titleText->SetCharacterSize(30);
	titleText->SetStyle(sf::Text::Bold);
	titleText->SetAlignment(ui::WidgetText::Alignment::Center);
	titleText->SetTextColor(sf::Color::White);
	m_mainWindow->AddWidget(std::move(titleText));

	// Create a button in the center of the screen
	auto button = std::make_unique<ui::WidgetButton>(860, 500, 300, 300);
	button->LoadImage("FluxNeurals.png");
	button->SetText("Test Button");
	button->SetOnClickCallback([]()
		{
			// Button click callback - you can add functionality here
		});
	m_mainWindow->AddWidget(std::move(button));


	// Create Gametime at right corner
	m_gameTimeText = std::make_unique<ui::WidgetText>(1700, 100, "GametimeInitText");
	m_gameTimeText->SetCharacterSize(20);
	m_gameTimeText->SetStyle(sf::Text::Bold);
	m_gameTimeText->SetAlignment(ui::WidgetText::Alignment::Center);
	m_gameTimeText->SetTextColor(sf::Color::White);

	m_mainWindow->AddWidget(std::move(m_gameTimeText));
	// Add a copy of the widget to the main window
//m_mainWindow->AddWidget(std::make_unique<ui::WidgetText>(*m_gameTimeText));

}

void Application::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastApplicationUpdate = sf::Time::Zero;

	while (m_renderContext->isOpen())
	{
		sf::Time delta = clock.restart();
		timeSinceLastApplicationUpdate += delta;

		ApplicationUpdate(timeSinceLastApplicationUpdate);
		timeSinceLastApplicationUpdate = sf::Time::Zero;

		InputHandle();
		DisplayHandle();
	}
}

void Application::ApplicationUpdate(sf::Time delta)
{
	//DebugLog("Application ApplicationUpdate called: " + std::to_string(delta.asSeconds()) + " seconds", DebugType::Message);

	TotalGameTimeUpdate(delta);

	//ApplicationUpdate Market
	if (m_stockMarket)
	{
		m_stockMarket->StockMarketUpdate(delta);
	}
	else
	{
		//DebugLog("StockMarket not exist", DebugType::Error);
	}
}

void Application::TotalGameTimeUpdate(sf::Time& delta)
{
	s_totalGameTime += delta.asSeconds();
	if (m_gameTimeText)
	{
		//DebugLog("Gametime: " + std::to_string(totalGameTime) + " s", DebugType::Message);

		m_gameTimeText->SetText("Gametime: " + std::to_string(s_totalGameTime) + "s");
	}
	else
	{
		//DebugLog("Timetext not exist", DebugType::Error);
	}
}

void Application::DisplayHandle()
{

	m_renderContext->clear();
	{
		m_mainWindow->Draw(*m_renderContext);
	}
	m_renderContext->display();
}

void Application::InputHandle()
{
	InputEvent event;
	while (m_renderContext->pollEvent(event))
	{
		if (event.type == InputEvent::Closed)
		{
			m_renderContext->close();
		}
		else
		{
			m_mainWindow->ProcessInput(event);
		}
	}
}

ui::Window* Application::GetMainWindow() const
{
	return m_mainWindow.get();
}

void Application::SetDataPath(const char* dataPath)
{
	s_dataPath = dataPath;
}

void Application::SetAssetsPath(const char* assetsPath)
{
	s_assetsPath = assetsPath;
}

