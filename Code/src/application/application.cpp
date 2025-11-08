#include "pch.h"
#include "application.h" // Own header (already includes needed UI + other headers)
#include "stockMarket.h" // StockMarket used directly in StockMarketSetup()
#include "inventory.h"   // Inventory used directly in InventorySetup()
#include "utilTools.h"   // Utility functions used in this translation unit
#include <sstream>
#include <iomanip>
#include <cmath>

// Static member definitions
std::string Application::s_dataPath;
std::string Application::s_assetsPath;

Application::Application()
	: m_mainMenuContainer(nullptr)
	, m_subMenuContainer(nullptr)
	, m_gameTimeText(nullptr)
{
}
Application::~Application() = default;

double Application::s_totalGameTime = 0.0; // Initialize total game time to 0 (double precision)
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
		// Initialize UI and add widgets to the root container
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

// void Application::InitializeUI() moved to applicationUI.cpp

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
	s_totalGameTime += static_cast<double>(delta.asSeconds());
	if (m_gameTimeText)
	{
		// Display only whole seconds (truncated)
		m_gameTimeText->SetText(std::to_string(static_cast<int>(s_totalGameTime)) + " sec");
	}
}

void Application::DisplayHandle()
{
	m_renderContext->clear();
	if (m_rootWidgetContainer)
	{
		m_rootWidgetContainer->Draw(*m_renderContext);
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
			// Process input through the root widget container
			if (m_rootWidgetContainer)
			{
				m_rootWidgetContainer->ProcessInput(event);
			}
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

