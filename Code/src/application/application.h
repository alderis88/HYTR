#pragma once
#include "../framework/WidgetText.h"
#include "../framework/window.h"
#include "../framework/WidgetImage.h"
#include "../framework/WidgetButton.h"
#include "../framework/WidgetContainer.h"

#include "inventory.h"
#include "utilTools.h"
#include "applicationUI.h"
#include "pch.h"

class Inventory;
class StockMarket; // forward declaration to avoid circular include
class ApplicationUI; // forward declaration

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
	void UI_InitializeGameTimeWidget();
	void UI_DebugContainers();
	void SetVideoSettings();

	void Run();
	void ApplicationUpdate(sf::Time delta);

	ui::Window* GetMainWindow() const;
	Inventory* GetPlayerInventory() const;
	ApplicationUI* GetApplicationUI() const;

	static std::string s_dataPath;
	static std::string s_assetsPath;

	// Public control flags / speed used by systems (made public)
	static float s_globalTimeMultiplier; // Global time multiplier for all game systems
	static float s_previousTimeMultiplier; // Previous time multiplier for pause/unpause functionality

	// Shared random number generator
	static std::mt19937& GetRandomGenerator();

	// Public system references for easy access by subsystems
	std::unique_ptr< Inventory > m_playerInventory;
	std::unique_ptr< StockMarket > m_stockMarket;

private:
	void SetDataPath(const char* dataPath);
	void SetAssetsPath(const char* assetsPath);

	void SetupInventory();
	void SetupStockMarket();
	void SetupCustomCursor();

	void TotalGameTimeUpdate(sf::Time& delta);
	void UpdateInputMode();
	void UpdateGamepadCursor(sf::Time delta);

	void DisplayHandle();
	void InputHandle();
	void HandleTestTrading(sf::Keyboard::Key key, bool isShiftPressed);

	std::unique_ptr< ui::Window > m_mainWindow;
	std::unique_ptr< RenderContext > m_renderContext;

	// Custom cursor
	sf::Texture m_cursorTexture; // Texture for custom cursor
	sf::Sprite m_cursorSprite;   // Sprite for custom cursor

	// Input mode handling
	enum class InputMode { Mouse, Gamepad };
	InputMode m_currentInputMode;              // Current input mode
	sf::Vector2f m_gamepadCursorPosition;      // Virtual cursor position for gamepad
	sf::Vector2i m_lastMousePosition;          // Last recorded mouse position
	float m_gamepadCursorSpeed;                // Speed multiplier for gamepad cursor movement
	unsigned int m_gamepadId;                  // ID of the connected gamepad (0-7)

	// Static variables
	static float s_totalGameTime; // Total game time in seconds (double precision)

	// UI variables
	std::unique_ptr<ApplicationUI> m_applicationUI; // UI management class
	std::unique_ptr<ui::WidgetContainer> m_rootWidgetContainer; // Root container for all widgets
	ui::WidgetContainer* m_monitorMenuContainer; // Pointer to monitor menu container (owned by root)
	ui::WidgetContainer* m_monitor1Container; // Pointer to monitor 1 container (owned by applicationUI)
	ui::WidgetText* m_gameTimeText; // Pointer to game time text widget (owned by container)
};

