#include "pch.h"
#include "../framework/window.h"
#include "application.h" // Own header (already includes needed UI + other headers)
#include "stockMarket.h" // StockMarket used directly in SetupStockMarket()
#include "inventory.h"   // Inventory used directly in SetupInventory()
#include "utilTools.h"   // Utility functions used in this translation unit
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <random>

// Static member definitions
std::string Application::s_dataPath;
std::string Application::s_assetsPath;
float Application::s_totalGameTime = 0.0f;
float Application::s_globalTimeMultiplier = 1.0f;
float Application::s_previousTimeMultiplier = 1.0f;

/// @brief Constructor - initializes all member variables to default values
/// Initializes UI pointers, input mode settings, and gamepad cursor configuration
Application::Application()
  : m_applicationUI(nullptr)
  , m_monitorMenuContainer(nullptr)
  , m_gameTimeText(nullptr)
  , m_currentInputMode(InputMode::Mouse)
  , m_gamepadCursorPosition(960.0f, 540.0f) // Center of 1920x1080 screen
  , m_lastMousePosition(0, 0)
  , m_gamepadCursorSpeed(500.0f) // pixels per second
  , m_gamepadId(0)
{
}
/// @brief Destructor - cleans up resources and releases memory
/// Uses default destruction for all smart pointers and SFML objects
Application::~Application() = default;

/// @brief Gets a shared random number generator instance
/// @return Reference to a static Mersenne Twister random number generator
/// Provides a thread-safe, high-quality random number generator for the entire application
std::mt19937& Application::GetRandomGenerator()
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	return generator;
}

/// @brief Main initialization function for the entire application
/// Sets up video settings, file paths, game systems, and UI components
/// Called once at application startup to prepare all subsystems
void Application::Initialize()
{
	// Configure video settings (window size, framerate, etc.)
	SetVideoSettings();

	// Set up file system paths for data and assets
	SetDataPath("../../data/");
	SetAssetsPath("../../assets/");

	// Initialize core game systems
	SetupStockMarket();	
	SetupCustomCursor();

	// Create main window and initialize UI system
	if (m_mainWindow = std::make_unique<ui::Window>())
	{
		// Create and initialize UI management class
		m_applicationUI = std::make_unique<ApplicationUI>();
		m_applicationUI->InitializeUI(this);

		// Get root container from UI manager
		m_rootWidgetContainer = std::move(m_applicationUI->GetRootContainer());

		// Get UI component pointers for direct access
		m_monitorMenuContainer = m_applicationUI->GetMonitorMenuContainer();
		m_gameTimeText = m_applicationUI->GetGameTimeText();
	}
	SetupInventory();
	
	// Initialize inventory buttons after setup
	if (m_applicationUI)
	{
		m_applicationUI->UpdateInventoryVerticalButtons();
	}
}

/// @brief Configures video and rendering settings
/// Creates the main render context with specified resolution and framerate
/// Sets up the SFML window with title and performance parameters
void Application::SetVideoSettings()
{
	// Create 1920x1080 window with specified title
	m_renderContext = std::make_unique<RenderContext>(sf::VideoMode(1920, 1080), "Hyper Trade");
	// Limit framerate to 30 FPS for consistent performance
	m_renderContext->setFramerateLimit(30);
}

/// @brief Initializes the stock market system
/// Creates and configures the stock market subsystem with products, vendors, and news
/// Loads JSON data and sets up market simulation parameters
void Application::SetupStockMarket()
{
	// Create stock market instance
	m_stockMarket = std::make_unique<StockMarket>();
	// Load market data and initialize trading system with application reference
	m_stockMarket->InitializeStockMarket(this);
}

/// @brief Initializes the player inventory system
/// Creates and configures the inventory subsystem for item management
/// Sets up initial inventory state and item storage capabilities
void Application::SetupInventory()
{
	// Create player inventory instance
	m_playerInventory = std::make_unique<Inventory>();
	// Initialize inventory with default settings and application reference
	m_playerInventory->InventoryInitialize(this);
}

/// @brief Sets up custom cursor graphics and input mode initialization
/// Loads cursor texture, configures sprite scaling, and initializes dual input mode system
/// Supports both mouse and gamepad cursor control with automatic mode switching
void Application::SetupCustomCursor()
{
	// Load custom cursor texture from assets
	std::string cursorPath = s_assetsPath + "Cursor2.png";

	if (!m_cursorTexture.loadFromFile(cursorPath))
	{
		// Handle error - cursor texture not found
		//DebugLog("Failed to load cursor texture from: " + cursorPath, DebugType::Error);
		return;
	}

	// Setup cursor sprite with loaded texture
	m_cursorSprite.setTexture(m_cursorTexture);

	// Calculate scale to make cursor exactly 25x25 pixels regardless of source image size
	sf::Vector2u textureSize = m_cursorTexture.getSize();
	float scaleX = 25.0f / static_cast<float>(textureSize.x);
	float scaleY = 25.0f / static_cast<float>(textureSize.y);
	m_cursorSprite.setScale(scaleX, scaleY);

	// Set cursor origin to top-left corner for precise positioning
	m_cursorSprite.setOrigin(0, 0);

	// Initialize dual input mode system (mouse/gamepad)
	if (m_renderContext)
	{
		// Set gamepad cursor to screen center as starting position
		sf::Vector2u windowSize = m_renderContext->getSize();
		m_gamepadCursorPosition.x = static_cast<float>(windowSize.x) / 2.0f;
		m_gamepadCursorPosition.y = static_cast<float>(windowSize.y) / 2.0f;

		// Store initial mouse position for input mode detection
		m_lastMousePosition = sf::Mouse::getPosition(*m_renderContext);

		// Hide system cursor since we're using custom rendering
		m_renderContext->setMouseCursorVisible(false);
	}
}

/// @brief Detects and switches between Mouse and Gamepad input modes automatically
/// Monitors mouse movement and gamepad analog stick input to determine active input device
/// Provides seamless switching with cursor position continuity between modes
void Application::UpdateInputMode()
{
	if (!m_renderContext)
		return;

	// Detect mouse movement and switch to Mouse mode if movement detected
	sf::Vector2i currentMousePos = sf::Mouse::getPosition(*m_renderContext);
	if (currentMousePos != m_lastMousePosition)
	{
		if (m_currentInputMode != InputMode::Mouse)
		{
			m_currentInputMode = InputMode::Mouse;
			//DebugLog("Switched to Mouse input mode", DebugType::Message);
		}
		m_lastMousePosition = currentMousePos;
	}

	// Detect gamepad analog stick movement and switch to Gamepad mode if input detected
	if (sf::Joystick::isConnected(m_gamepadId))
	{
		float axisX = sf::Joystick::getAxisPosition(m_gamepadId, sf::Joystick::X);
		float axisY = sf::Joystick::getAxisPosition(m_gamepadId, sf::Joystick::Y);

		// Apply deadzone threshold to prevent noise-triggered mode switches
		float deadzone = 15.0f; // 15% deadzone to filter out analog stick drift
		if (std::abs(axisX) > deadzone || std::abs(axisY) > deadzone)
		{
			if (m_currentInputMode != InputMode::Gamepad)
			{
				m_currentInputMode = InputMode::Gamepad;
				// Seamless transition: set gamepad cursor to current mouse position
				m_gamepadCursorPosition.x = static_cast<float>(currentMousePos.x);
				m_gamepadCursorPosition.y = static_cast<float>(currentMousePos.y);
				//DebugLog("Switched to Gamepad input mode", DebugType::Message);
			}
		}
	}
}

/// @brief Updates virtual gamepad cursor position based on analog stick input
/// @param delta Time elapsed since last frame for frame-rate independent movement
/// Processes gamepad analog stick input and moves virtual cursor with proper boundary clamping
void Application::UpdateGamepadCursor(sf::Time delta)
{
	// Only update in Gamepad mode and if gamepad is actually connected
	if (m_currentInputMode != InputMode::Gamepad || !sf::Joystick::isConnected(m_gamepadId))
		return;

	// Read left analog stick values (X and Y axes)
	float axisX = sf::Joystick::getAxisPosition(m_gamepadId, sf::Joystick::X);
	float axisY = sf::Joystick::getAxisPosition(m_gamepadId, sf::Joystick::Y);

	// Apply deadzone to prevent cursor drift from analog stick noise
	float deadzone = 15.0f; // 15% deadzone threshold
	if (std::abs(axisX) < deadzone) axisX = 0.0f;
	if (std::abs(axisY) < deadzone) axisY = 0.0f;

	// Normalize axis values from (-100, 100) to (-1, 1) range
	float moveX = axisX / 100.0f;
	float moveY = axisY / 100.0f;

	// Calculate frame-rate independent movement using delta time
	float speed = m_gamepadCursorSpeed * delta.asSeconds();
	m_gamepadCursorPosition.x += moveX * speed;
	m_gamepadCursorPosition.y += moveY * speed;

	// Clamp cursor position to stay within screen boundaries
	if (m_renderContext)
	{
		sf::Vector2u windowSize = m_renderContext->getSize();
		m_gamepadCursorPosition.x = std::max(0.0f, std::min(static_cast<float>(windowSize.x), m_gamepadCursorPosition.x));
		m_gamepadCursorPosition.y = std::max(0.0f, std::min(static_cast<float>(windowSize.y), m_gamepadCursorPosition.y));
	}
}

/// @brief Main application loop - handles timing, updates, input, and rendering
/// Runs continuously until window is closed, managing frame timing and system updates
/// Coordinates all subsystems including input detection, game logic, and display rendering
void Application::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastApplicationUpdate = sf::Time::Zero;

	// Main game loop - continues until window close is requested
	while (m_renderContext->isOpen())
	{
		// Calculate frame time delta for frame-rate independent updates
		sf::Time delta = clock.restart();
		timeSinceLastApplicationUpdate += delta;

		// Update all game systems with accumulated time
		ApplicationUpdate(timeSinceLastApplicationUpdate);
		timeSinceLastApplicationUpdate = sf::Time::Zero;

		// Process user input (keyboard, mouse, gamepad)
		InputHandle();
		// Render all visual elements to screen
		DisplayHandle();
	}
}

/// @brief Updates all application subsystems with frame timing
/// @param delta Time elapsed since last update for frame-rate independent calculations
/// Coordinates input mode detection, cursor movement, game time, and market simulation updates
void Application::ApplicationUpdate(sf::Time delta)
{
	//DebugLog("Application ApplicationUpdate called: " + std::to_string(delta.asSeconds()) + " seconds", DebugType::Message);

	// Apply global time multiplier to delta for consistent time scaling across all systems
	sf::Time scaledDelta = sf::seconds(delta.asSeconds() * s_globalTimeMultiplier);

	// Update dual input system (mouse/gamepad mode detection and cursor movement)
	UpdateInputMode();
	UpdateGamepadCursor(delta); // Cursor movement not affected by time scaling

	// Update global game time tracking for UI display
	TotalGameTimeUpdate(scaledDelta);

	// Update stock market simulation system with scaled time
	if (m_stockMarket)
	{
		m_stockMarket->StockMarketUpdate(scaledDelta);
	}
	else
	{
		//DebugLog("StockMarket not exist", DebugType::Error);
	}

	// Update rolling text animation and cycle progress bar (not affected by global time multiplier)
	if (m_applicationUI)
	{
		m_applicationUI->UpdateApplicationUI(scaledDelta);
		m_applicationUI->UpdateCycleProgressBar();
	}
}

/// @brief Updates and displays total elapsed game time
/// @param delta Time elapsed since last frame to accumulate into total game time
/// Maintains high-precision game time tracking and updates UI display with whole seconds
void Application::TotalGameTimeUpdate(sf::Time& delta)
{
	// Accumulate elapsed time with high precision (float) - already scaled by global multiplier
	s_totalGameTime += static_cast<float>(delta.asSeconds());

	// Update UI display if game time widget exists
	if (m_gameTimeText)
	{
		// Display only whole seconds (truncated) for clean UI appearance
		m_gameTimeText->SetText(std::to_string(static_cast<int>(s_totalGameTime)) + " sec");
	}
}


/// @brief Handles all rendering operations for the current frame
/// Clears screen, draws UI elements, renders dual-mode cursor, and presents final image
/// Supports both mouse and gamepad cursor rendering based on current input mode
void Application::DisplayHandle()
{
	// Clear previous frame with background color
	m_renderContext->clear();

	// Render all UI widgets through the root container
	if (m_rootWidgetContainer)
	{
		m_rootWidgetContainer->Draw(*m_renderContext);
	}

	// Render custom cursor with dual input mode support
	if (m_cursorTexture.getSize().x > 0) // Ensure cursor texture is loaded
	{
		sf::Vector2f cursorPos;

		if (m_currentInputMode == InputMode::Mouse)
		{
			// Mouse mode: use real-time mouse position from system
			sf::Vector2i mousePos = sf::Mouse::getPosition(*m_renderContext);
			cursorPos.x = static_cast<float>(mousePos.x);
			cursorPos.y = static_cast<float>(mousePos.y);
		}
		else // InputMode::Gamepad
		{
			// Gamepad mode: use virtual cursor position controlled by analog stick
			cursorPos = m_gamepadCursorPosition;
		}

		// Position and render custom cursor sprite
		m_cursorSprite.setPosition(cursorPos.x, cursorPos.y);
		m_renderContext->draw(m_cursorSprite);
	}

	// Present completed frame to screen
	m_renderContext->display();
}

/// @brief Processes SFML input events for window and UI interaction
/// Handles window close events, Escape key shutdown, global time control (+/-), pause/unpause (Space), and forwards UI input to the widget system
/// Note: Gamepad and mouse movement detection handled separately in UpdateInputMode()
void Application::InputHandle()
{
	InputEvent event;
	// Poll all pending SFML events from the window
	while (m_renderContext->pollEvent(event))
	{
		if (event.type == InputEvent::Closed ||
		   (event.type == InputEvent::KeyPressed && event.key.code == sf::Keyboard::Escape))
		{
			// User clicked window close button or pressed Escape key - initiate application shutdown
			m_renderContext->close();
		}
		else if (event.type == InputEvent::KeyPressed &&
		         (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Subtract))
		{
			// Global time multiplier control with predefined intervals
			static const float timeLevels[] = { 0.1f, 0.5f, 1.0f, 2.0f, 5.0f, 20.0f };
			static const int maxLevelIndex = sizeof(timeLevels) / sizeof(timeLevels[0]) - 1;

			// Find current time multiplier level index
			int currentIndex = 0;
			for (int i = 0; i <= maxLevelIndex; ++i)
			{
				if (std::abs(s_globalTimeMultiplier - timeLevels[i]) < 0.01f)
				{
					currentIndex = i;
					break;
				}
			}

			// Adjust time multiplier based on key pressed
			if (event.key.code == sf::Keyboard::Add && currentIndex < maxLevelIndex)
			{
				// Plus key pressed - increase to next time level
				float oldMultiplier = s_globalTimeMultiplier;
				s_globalTimeMultiplier = timeLevels[currentIndex + 1];
				DebugLog("Global time multiplier increased from " + std::to_string(oldMultiplier) + " to " + std::to_string(s_globalTimeMultiplier), DebugType::Message);
			}
			else if (event.key.code == sf::Keyboard::Subtract && currentIndex > 0)
			{
				// Minus key pressed - decrease to previous time level
				float oldMultiplier = s_globalTimeMultiplier;
				s_globalTimeMultiplier = timeLevels[currentIndex - 1];
				DebugLog("Global time multiplier decreased from " + std::to_string(oldMultiplier) + " to " + std::to_string(s_globalTimeMultiplier), DebugType::Message);
			}
		}
		else if (event.type == InputEvent::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			// Space key pressed - toggle pause/unpause by setting time multiplier to 0 or restoring previous value
			if (s_globalTimeMultiplier > 0.0f)
			{
				// Game is running - pause by saving current speed and setting to 0
				s_previousTimeMultiplier = s_globalTimeMultiplier;
				s_globalTimeMultiplier = 0.0f;
				DebugLog("Game paused - time multiplier set to 0, previous value: " + std::to_string(s_previousTimeMultiplier), DebugType::Message);
			}
			else
			{
				// Game is paused - unpause by restoring previous speed
				s_globalTimeMultiplier = s_previousTimeMultiplier;
				DebugLog("Game unpaused - time multiplier restored to: " + std::to_string(s_globalTimeMultiplier), DebugType::Message);
			}
		}
		else if (event.type == InputEvent::KeyPressed)
		{
			// Test trading hotkeys: 1-5 to buy, Shift+1-5 to sell
			bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
			
			if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num5)
			{
				HandleTestTrading(event.key.code, isShiftPressed);
			}
			else
			{
				// Forward all other input events to UI widget system for processing
				if (m_rootWidgetContainer)
				{
					m_rootWidgetContainer->ProcessInput(event);
				}
			}
		}
		else
		{
			// Forward all other input events to UI widget system for processing
			if (m_rootWidgetContainer)
			{
				m_rootWidgetContainer->ProcessInput(event);
			}
		}
	}
}

/// @brief Gets pointer to the main application window
/// @return Pointer to the main UI window object, or nullptr if not initialized
/// Used by subsystems that need access to the main window for UI operations
ui::Window* Application::GetMainWindow() const
{
	return m_mainWindow.get();
}

/// @brief Get access to the player inventory system
/// @return Pointer to the player inventory object, or nullptr if not initialized
/// Used by subsystems that need to modify or query player inventory
Inventory* Application::GetPlayerInventory() const
{
	return m_playerInventory.get();
}

/// @brief Get access to the application UI system
/// @return Pointer to the application UI object, or nullptr if not initialized
/// Used by subsystems that need to update UI displays
ApplicationUI* Application::GetApplicationUI() const
{
	return m_applicationUI.get();
}

/// @brief Handle test trading functionality with keyboard shortcuts
/// @param key The keyboard key pressed (Num1-Num5)
/// @param isShiftPressed Whether Shift key is held down
/// @details Keys 1-5 buy 1 unit, Shift+1-5 sell 1 unit of corresponding products:
///          1/Shift+1 = TRI (Tritanium), 2/Shift+2 = NFX (Neuroflux), 3/Shift+3 = ZER (Zeromass),
///          4/Shift+4 = LUM (Lumirite), 5/Shift+5 = NAN (Nanochip)
void Application::HandleTestTrading(sf::Keyboard::Key key, bool isShiftPressed)
{
	// Safety checks
	if (!m_playerInventory || !m_stockMarket)
		return;

	// Map keyboard keys to product IDs
	std::string productIds[5] = { "TRI", "NFX", "ZER", "LUM", "NAN" };
	std::string productNames[5] = { "Tritanium", "Neuroflux", "Zeromass", "Lumirite", "Nanochip" };
	
	// Convert key to array index (Num1=0, Num2=1, etc.)
	int productIndex = static_cast<int>(key) - static_cast<int>(sf::Keyboard::Num1);
	
	if (productIndex < 0 || productIndex >= 5)
		return;
		
	const std::string& productId = productIds[productIndex];
	const std::string& productName = productNames[productIndex];
	
	if (isShiftPressed)
	{
		// Shift+Number: Sell 1 unit using StockMarket
		uint32_t currentQuantity = m_playerInventory->GetProductQuantity(productId);
		if (currentQuantity > 0)
		{
			m_stockMarket->SellForStock(productId, 1);
			uint32_t remainingQuantity = m_playerInventory->GetProductQuantity(productId);
			DebugLog("TEST SELL: Sold 1 " + productName + " (" + productId + ") to market. Remaining: " + std::to_string(remainingQuantity), DebugType::Message);
		}
		else
		{
			DebugLog("TEST SELL: Cannot sell " + productName + " (" + productId + ") - no units in inventory!", DebugType::Warning);
		}
	}
	else
	{
		// Number only: Buy 1 unit using StockMarket
		bool success = m_stockMarket->BuyFromStock(productId, 1);
		if (success)
		{
			uint32_t newQuantity = m_playerInventory->GetProductQuantity(productId);
			DebugLog("TEST BUY: Bought 1 " + productName + " (" + productId + ") from market. Total: " + std::to_string(newQuantity), DebugType::Message);
		}
		else
		{
			DebugLog("TEST BUY: Cannot buy " + productName + " (" + productId + ") - not enough stock or funds!", DebugType::Warning);
		}
	}
	

}

/// @brief Sets the global data directory path for JSON and data file loading
/// @param dataPath Relative or absolute path to data directory (e.g., "../../data/")
/// Used by all subsystems to locate JSON files for game content (stocks, vendors, etc.)
void Application::SetDataPath(const char* dataPath)
{
	s_dataPath = dataPath;
}

/// @brief Sets the global assets directory path for textures, fonts, and media files
/// @param assetsPath Relative or absolute path to assets directory (e.g., "../../assets/")
/// Used by all subsystems to locate graphics, fonts, and audio resources
void Application::SetAssetsPath(const char* assetsPath)
{
	s_assetsPath = assetsPath;
}

