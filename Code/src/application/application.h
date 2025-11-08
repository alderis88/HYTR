#pragma once
#include "../framework/WidgetText.h"
#include "../framework/window.h"
#include "../framework/WidgetImage.h"
#include "../framework/WidgetButton.h"

#include "inventory.h"
#include "utilTools.h"
#include "pch.h"

class Inventory;
class StockMarket; // forward declaration to avoid circular include

namespace ui
{
	class Window;
}

class Application final
{
public:
	Application();
	~Application();

	void Initialize();
	void InitializeUI();
	void SetVideoSettings();
	void StockMarketSetup();
	void Run();
	void ApplicationUpdate(sf::Time delta);

	ui::Window* GetMainWindow() const;

	static std::string s_dataPath;
	static std::string s_assetsPath;

	// Public control flags / speed used by systems (made public)
	static bool s_pauseGame;
	static float s_stockGameSpeed;
	
	// Shared random number generator
	static std::mt19937& GetRandomGenerator();

private:
	void SetDataPath(const char* dataPath);
	void SetAssetsPath(const char* assetsPath);
	void InventorySetup();

	void TotalGameTimeUpdate(sf::Time& delta);

	void DisplayHandle();
	void InputHandle();

	std::unique_ptr< ui::Window > m_mainWindow;
	std::unique_ptr< RenderContext > m_renderContext;

	std::unique_ptr< Inventory > m_playerInventory;
	std::unique_ptr< StockMarket > m_stockMarket;

	// Static variables
	static float s_totalGameTime; // Total game time in seconds


	// UI variables
	std::unique_ptr<ui::WidgetText> m_gameTimeText; // Widget to display game time
};

