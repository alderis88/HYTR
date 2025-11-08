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
	uint32_t m_currentPriceWithoutPlayerImpact;
	uint32_t m_currentPrice;
	float m_currentPlayerImpact;
	bool m_trendIncreased;

};

class Application; // forward declaration

class Inventory final
{
public:
	Inventory();
	~Inventory();

	void InventoryInitialize(Application* app);

	// === Inventory Management ===
	uint32_t GetCurrentMoney() const;
	void SetCurrentMoney(uint32_t money);
	uint32_t GetProductQuantity(const std::string& productId) const;
	uint32_t GetTotalInventoryValue() const;
	float GetCurrentInventoryVolume() const;
	float GetMaxInventoryVolume() const;
	const std::vector<StockProduct>& GetPlayerProducts() const;

	// === Product Management ===
	void AddProduct(const std::string& productId, uint32_t quantity);
	void RemoveProduct(const std::string& productId, uint32_t quantity);

private:
	StockProduct* FindProduct(const std::string& productId);
	void LoadInventoryProducts(const std::string& path);

	// === System References ===
	Application* m_application = nullptr;       ///< Reference to main application instance

	uint32_t m_currentMoney;
	float m_currentInventoryVolume;
	std::vector<StockProduct> m_playerProducts;
	
	static constexpr float s_maxInventoryVolume = 1000.0f;
};
