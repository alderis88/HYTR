#pragma once
#include "pch.h"
#include "inventory.h"

class StockMarket
{
public:
	void InitializeStockMarket();
	void StockMarketUpdate(sf::Time delta);
	void StockMarketCycleStep();
	void CycleTimerUpdate();

	void LoadStockProducts(const std::string& path);
	void InitializeProductValues();
	void CalculateProductPrice(StockProduct& product);
	void ReducePlayerImpact(StockProduct& product);
	void ProductStockReplenishment(StockProduct& product);

	float currentCyleTime = 0.0f;
	uint32_t cycleCount = 0;

private:
	std::vector<StockProduct> m_stockProducts;
	
	static constexpr float s_randomPriceInfluenceFactor = 0.025f;
	static constexpr float stockCycleTime = 5.0f;
};
