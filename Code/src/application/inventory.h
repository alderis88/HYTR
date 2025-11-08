#pragma once

enum class RarityLevel : char
{
	Rare,
	Normal,
	Common,

	MAX
};

struct StockProduct final
{
	//Json defined members
	std::string m_id;
	std::string m_name;
	float m_volume;
	uint32_t m_basePrice;
	float m_playerImpact;
	float m_minPrice;
	float m_maxPrice;
	std::vector<float> m_trends;
	RarityLevel m_itemRarity;
	uint32_t m_stackReplenishment;
	float m_sellStackRatio;
	uint32_t m_maxQuantity;

	//Inventory defined members
	uint32_t m_quantity;
	uint32_t m_trendPointer;
	uint32_t m_currentPrice;
	float m_currentPlayerImpact;
	bool m_trendIncreased;

};

class Inventory final
{
public:
	Inventory();
	~Inventory();

	void InventoryInitialize();

private:

	uint32_t m_currentMoney;
	std::vector<StockProduct> m_playerProducts;
};
