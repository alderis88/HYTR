#pragma once
#include "../framework/WidgetText.h"
#include "../framework/window.h"
#include "../framework/WidgetImage.h"
#include "../framework/WidgetButton.h"
#include "../framework/WidgetContainer.h"

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
	void InitializeContainersUI();
	void UI_InitializeRootContainer();
	void UI_InitializeMainMenuContainer();
	void UI_InitializeSubMenuContainer();
	void UI_InitializeGameTimeWidget();
	void UI_DebugContainers();
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
	static double s_totalGameTime; // Total game time in seconds (double precision)

	// UI variables
	std::unique_ptr<ui::WidgetContainer> m_rootWidgetContainer; // Root container for all widgets
	ui::WidgetContainer* m_mainMenuContainer; // Pointer to main menu container (owned by root)
	ui::WidgetContainer* m_subMenuContainer;  // Pointer to sub menu container (owned by root)
	ui::WidgetText* m_gameTimeText; // Pointer to game time text widget (owned by container)
};

