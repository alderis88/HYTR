#pragma once
#include "pch.h"
#include "inventory.h"

/// @brief Represents character personality traits for vendors
struct Personality
{
	uint32_t discipline;   ///< How disciplined the character is (0-10)
	uint32_t riskTaking;   ///< Willingness to take risks (0-10)
	uint32_t greed;        ///< Level of greed (0-10)
	uint32_t honor;        ///< Honor/integrity level (0-10)
};

/// @brief Represents a stock market vendor character
struct StockVendor final
{
	// JSON defined members - loaded from vendor_characters.json
	std::string m_id;                           ///< Unique vendor identifier
	std::string m_productId;                    ///< Associated product ID
	std::string m_name;                         ///< Vendor's real name
	std::string m_alias;                        ///< Vendor's alias/nickname
	std::string m_company;                      ///< Company name
	std::string m_role;                         ///< Position/role in company
	std::string m_profile;                      ///< Character background
	std::string m_appearance;                   ///< Physical description
	std::string m_mood;                         ///< Current mood/atmosphere
	std::vector<std::string> m_colorTheme;      ///< UI color scheme
	std::string m_quote;                        ///< Signature quote
	std::string m_style;                        ///< Visual style description
	Personality m_personality;                  ///< Personality traits
};

/// @brief Represents a market news item
struct News final
{
	std::string m_newsContent;      ///< The news text content
	uint32_t m_currentNewsIndex;    ///< Current index (not used in practice)
};

/// @brief Main stock market simulation class
/// Manages products, vendors, news, and market cycles
class StockMarket
{
public:
	// === Core System Functions ===
	/// @brief Initialize the entire stock market system
	void InitializeStockMarket();
	
	/// @brief Main update loop called every frame
	/// @param delta Time elapsed since last update
	void StockMarketUpdate(sf::Time delta);
	
	/// @brief Execute one market cycle (price updates, stock replenishment, etc.)
	void StockMarketCycleStep();
	
	/// @brief Update cycle timer (for debugging purposes)
	void CycleTimerUpdate();

	// === Data Loading Functions ===
	/// @brief Load stock products from JSON file
	/// @param path Path to item_products.json
	void LoadJsonStockProducts(const std::string& path);
	
	/// @brief Load vendor characters from JSON file
	/// @param path Path to vendor_characters.json
	void LoadJsonStockVendors(const std::string& path);
	
	/// @brief Load market news from JSON file
	/// @param path Path to news.json
	void LoadJsonNews(const std::string& path);
	
	/// @brief Get next news item in rotation (cyclical with shuffle)
	/// @return Pointer to next news item, nullptr if no news available
	News* GetNextNews();

	// === Product Management Functions ===
	/// @brief Initialize random values for all products (quantities, prices, trends)
	void InitializeProductValues();
	
	/// @brief Calculate and update current price for a product
	/// @param product Reference to the product to update
	void CalculateProductPrice(StockProduct& product);
	
	/// @brief Reduce player impact on product over time
	/// @param product Reference to the product to update
	void ReducePlayerImpact(StockProduct& product);
	
	/// @brief Replenish stock quantity for a product
	/// @param product Reference to the product to replenish
	void ProductStockReplenishment(StockProduct& product);

	// === Product Access Functions ===
	/// @brief Find a product by its ID
	/// @param productId The ID to search for
	/// @return Pointer to product if found, nullptr otherwise
	StockProduct* GetStockProductById(const std::string& productId);
	
	/// @brief Check if a buy transaction is possible
	/// @param productId ID of the product to buy
	/// @param desiredQuantity Amount player wants to buy
	/// @return true if transaction is valid, false if insufficient stock
	bool ValidateBuyFromStock(const std::string& productId, uint32_t desiredQuantity);
	
	/// @brief Execute a buy transaction (reduces stock)
	/// @param productId ID of the product to buy
	/// @param quantity Amount to buy
	/// @return true if successful, false if insufficient stock
	bool BuyFromStock(const std::string& productId, uint32_t quantity);
	
	/// @brief Execute a sell transaction (increases stock based on sellStackRatio)
	/// @param productId ID of the product to sell
	/// @param quantity Amount to sell
	void SellForStock(const std::string& productId, uint32_t quantity);

	// === Public State Variables ===
	float m_currentCycleTime = 0.0f;    ///< Current time within market cycle (seconds)
	uint32_t m_cycleCount = 0;           ///< Total number of completed market cycles

private:
	// === Core Data Collections ===
	std::vector<StockProduct> m_stockProducts;  ///< All available stock products
	std::vector<StockVendor> m_stockVendors;    ///< All vendor characters
	std::vector<News> m_news;                   ///< All market news items
	uint32_t m_newsIndex = 0;                   ///< Current news index for rotation
	
	// === Market Constants ===
	static constexpr float s_randomPriceInfluenceFactor = 0.025f;  ///< Max random price variation (±2.5%)
	static constexpr float s_stockCycleTime = 5.0f;                ///< Time between market cycles (seconds)
};
