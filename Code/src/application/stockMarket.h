#pragma once
#include "pch.h"
#include "inventory.h"

struct Personality
{
	uint32_t discipline;   ///< How disciplined the character is (0-10)
	uint32_t riskTaking;   ///< Willingness to take risks (0-10)
	uint32_t greed;        ///< Level of greed (0-10)
	uint32_t honor;        ///< Honor/integrity level (0-10)
};

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

struct News final
{
	std::string m_newsContent;      ///< The news text content
	uint32_t m_currentNewsIndex;    ///< Current index (not used in practice)
};

class Application; // forward declaration

class StockMarket
{
public:
	// === Core System Functions ===
	void InitializeStockMarket(Application* app);
	void StockMarketUpdate(sf::Time delta);
	void StockMarketCycleStep();
	void CycleTimerUpdate();

	// === Data Loading Functions ===
	void LoadJsonStockProducts(const std::string& path);
	void LoadJsonStockVendors(const std::string& path);
	void LoadJsonNews(const std::string& path);
	News* GetNextNews();

	// === Product Management Functions ===
	void InitializeProductValues();
	void CalculateProductPrice(StockProduct& product);
	void CalculateOnlyPlayerInfluenceChangePrice(StockProduct& product);
	void ReducePlayerImpact(StockProduct& product);
	void ProductStockReplenishment(StockProduct& product);

	// === Product Access Functions ===
	StockProduct* GetStockProductById(const std::string& productId);
	bool ValidateBuyFromStock(const std::string& productId, uint32_t desiredQuantity);
	bool BuyFromStock(const std::string& productId, uint32_t quantity);
	void SellForStock(const std::string& productId, uint32_t quantity);

	// === Current Product Management ===
	void SetCurrentProductID(const std::string& productId);

	// === Public State Variables ===
	float m_currentCycleTime = 0.0f;    	///< Current time within market cycle (seconds)
	uint32_t m_cycleCount = 0;           	///< Total number of completed market cycles
	std::string currentProductID;        	///< Currently selected product ID

private:
	// === System References ===
	Application* m_application = nullptr;       ///< Reference to main application instance

	// === Core Data Collections ===
	std::vector<StockProduct> m_stockProducts;  ///< All available stock products
	std::vector<StockVendor> m_stockVendors;    ///< All vendor characters
	std::vector<News> m_news;                   ///< All market news items
	uint32_t m_newsIndex = 0;                   ///< Current news index for rotation

	// === Market Constants ===
	static constexpr float s_randomPriceInfluenceFactor = 0.025f;  ///< Max random price variation (±2.5%)
	static constexpr float s_stockCycleTime = 5.0f;                ///< Time between market cycles (seconds)
};
