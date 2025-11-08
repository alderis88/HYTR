#pragma once
#include "pch.h"
#include "inventory.h"

struct Personality
{
	uint32_t discipline;
	uint32_t riskTaking;
	uint32_t greed;
	uint32_t honor;
};

struct StockVendor final
{
	//Json defined members
	std::string m_id;
	std::string m_productId;
	std::string m_name;
	std::string m_alias;
	std::string m_company;
	std::string m_role;
	std::string m_profile;
	std::string m_appearance;
	std::string m_mood;
	std::vector<std::string> m_colorTheme;
	std::string m_quote;
	std::string m_style;
	Personality m_personality;
};

struct News final
{
	std::string m_newsContent;
	uint32_t m_currentNewsIndex;
};

class StockMarket
{
public:
	void InitializeStockMarket();
	void StockMarketUpdate(sf::Time delta);
	void StockMarketCycleStep();
	void CycleTimerUpdate();

	void LoadJsonStockProducts(const std::string& path);
	void LoadJsonStockVendors(const std::string& path);
	void LoadJsonNews(const std::string& path);
	
	News* GetNextNews();

	void InitializeProductValues();
	void CalculateProductPrice(StockProduct& product);
	void ReducePlayerImpact(StockProduct& product);
	void ProductStockReplenishment(StockProduct& product);
	StockProduct* GetStockProductById(const std::string& productId);
	bool ValidateBuyFromStock(const std::string& productId, uint32_t desiredQuantity);
	bool BuyFromStock(const std::string& productId, uint32_t quantity);
	void SellForStock(const std::string& productId, uint32_t quantity);

	float m_currentCycleTime = 0.0f;
	uint32_t m_cycleCount = 0;

private:
	std::vector<StockProduct> m_stockProducts;
	std::vector<StockVendor> m_stockVendors;
	std::vector<News> m_news;
	uint32_t m_newsIndex = 0;
	
	static constexpr float s_randomPriceInfluenceFactor = 0.025f;
	static constexpr float s_stockCycleTime = 5.0f;
};
