#include "pch.h"
#include "inventory.h"
#include "application.h"
#include <algorithm>
#include <fstream>
#include <sstream>

Inventory::Inventory() = default;
Inventory::~Inventory() = default;

/// @brief Initialize inventory system with starting values and load products from JSON
/// @param app Reference to the main application instance
void Inventory::InventoryInitialize(Application* app)
{
	// Store application reference
	m_application = app;
	DebugLog("Inventory - Application reference set");

	//Starting money
	m_currentMoney = 10000;
	
	//Starting inventory volume
	m_currentInventoryVolume = 0.0f;
	
	// Load player products from JSON
	std::ostringstream pathBuilder;
	pathBuilder << Application::s_dataPath << "item_products.json";
	LoadInventoryProducts(pathBuilder.str());
	
	DebugLog("Player Inventory initialized with " + std::to_string(m_playerProducts.size()) + " products, each with quantity 1. Total volume: " + 
			 std::to_string(m_currentInventoryVolume) + "/" + std::to_string(s_maxInventoryVolume));
}

/// @brief Load inventory products from JSON file
/// @param path Path to the JSON file containing product data
void Inventory::LoadInventoryProducts(const std::string& path)
{
	DebugLog("Loading Player Products from: " + path);
	std::ifstream stream(path);
	std::string fileData((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	Json::Document document;
	document.Parse(fileData.c_str());
	assert(!document.HasParseError());

	assert(document.IsObject());

	//products
	assert(document.HasMember("products"));
	const Json::Value& arrayObject = document["products"];
	assert(arrayObject.IsArray());
	for (Json::SizeType i = 0; i < arrayObject.Size(); i++)
	{
		StockProduct newProduct;

		// id
		assert(arrayObject[i].HasMember("id"));
		assert(arrayObject[i]["id"].IsString());
		newProduct.m_id = arrayObject[i]["id"].GetString();

		// name
		assert(arrayObject[i].HasMember("name"));
		assert(arrayObject[i]["name"].IsString());
		newProduct.m_name = arrayObject[i]["name"].GetString();

		//volume
		assert(arrayObject[i].HasMember("volume"));
		assert(arrayObject[i]["volume"].IsFloat());
		newProduct.m_volume = arrayObject[i]["volume"].GetFloat();

		//basePrice
		assert(arrayObject[i].HasMember("basePrice"));
		assert(arrayObject[i]["basePrice"].IsInt());
		newProduct.m_basePrice = static_cast<uint32_t>(arrayObject[i]["basePrice"].GetInt());

		//volume (duplicate - keeping as in original)
		assert(arrayObject[i].HasMember("volume"));
		assert(arrayObject[i]["volume"].IsFloat());
		newProduct.m_volume = arrayObject[i]["volume"].GetFloat();

		//playerImpact
		assert(arrayObject[i].HasMember("playerImpact"));
		assert(arrayObject[i]["playerImpact"].IsFloat());
		newProduct.m_playerImpact = arrayObject[i]["playerImpact"].GetFloat();

		//minPrice
		assert(arrayObject[i].HasMember("minPrice"));
		assert(arrayObject[i]["minPrice"].IsFloat());
		newProduct.m_minPrice = arrayObject[i]["minPrice"].GetFloat();

		//maxPrice
		assert(arrayObject[i].HasMember("maxPrice"));
		assert(arrayObject[i]["maxPrice"].IsFloat());
		newProduct.m_maxPrice = arrayObject[i]["maxPrice"].GetFloat();

		//m_trends
		assert(arrayObject[i].HasMember("trends"));
		const Json::Value& internalArrayObject = arrayObject[i]["trends"];
		assert(internalArrayObject.IsArray());
		for (Json::SizeType j = 0; j < internalArrayObject.Size(); j++)
		{
			assert(internalArrayObject[j].IsFloat());
			newProduct.m_trends.push_back(internalArrayObject[j].GetFloat());
		}

		// itemRarity (now read as string and converted to enum)
		assert(arrayObject[i].HasMember("itemRarity"));
		assert(arrayObject[i]["itemRarity"].IsString());
		{
			std::string rarityStr = arrayObject[i]["itemRarity"].GetString();
			// map string to enum
			if (rarityStr == "Rare")
			{
				newProduct.m_itemRarity = RarityLevel::Rare;
			}
			else if (rarityStr == "Normal")
			{
				newProduct.m_itemRarity = RarityLevel::Normal;
			}
			else if (rarityStr == "Common")
			{
				newProduct.m_itemRarity = RarityLevel::Common;
			}
			else
			{
				// Unknown value: log and fallback to Common
				DebugLog("Unknown itemRarity value: " + rarityStr + " ; defaulting to Common", DebugType::Warning);
				newProduct.m_itemRarity = RarityLevel::Common;
			}
		}

		//stackReplenishment
		assert(arrayObject[i].HasMember("stackReplenishment"));
		assert(arrayObject[i]["stackReplenishment"].IsInt());
		newProduct.m_stackReplenishment = static_cast<uint32_t>(arrayObject[i]["stackReplenishment"].GetInt());

		//sellStackRatio
		assert(arrayObject[i].HasMember("sellStackRatio"));
		assert(arrayObject[i]["sellStackRatio"].IsFloat());
		newProduct.m_sellStackRatio = arrayObject[i]["sellStackRatio"].GetFloat();

		//maxQuantity
		assert(arrayObject[i].HasMember("maxQuantity"));
		assert(arrayObject[i]["maxQuantity"].IsInt());
		newProduct.m_maxQuantity = static_cast<uint32_t>(arrayObject[i]["maxQuantity"].GetInt());

		//productInfo
		assert(arrayObject[i].HasMember("productInfo"));
		assert(arrayObject[i]["productInfo"].IsString());
		newProduct.m_productInfo = arrayObject[i]["productInfo"].GetString();

		// Initialize inventory-specific values (different from stock market)
		newProduct.m_quantity = 1; // Each player product starts with quantity 1
		newProduct.m_trendPointer = 0;
		newProduct.m_currentPrice = newProduct.m_basePrice;
		newProduct.m_currentPlayerImpact = 0.0f;
		newProduct.m_trendIncreased = false;

		// Add to current inventory volume (quantity * volume)
		m_currentInventoryVolume += newProduct.m_quantity * newProduct.m_volume;

		m_playerProducts.push_back(std::move(newProduct));
	}
}

// === Inventory Management ===

/// @brief Get current player money
/// @return Current money amount
uint32_t Inventory::GetCurrentMoney() const
{
	return m_currentMoney;
}

/// @brief Set player money amount
/// @param money New money amount
void Inventory::SetCurrentMoney(uint32_t money)
{
	m_currentMoney = money;
}

/// @brief Get quantity of a specific product in inventory
/// @param productId ID of the product to check
/// @return Quantity owned, 0 if product not found
uint32_t Inventory::GetProductQuantity(const std::string& productId) const
{
	auto it = std::find_if(m_playerProducts.begin(), m_playerProducts.end(),
		[&productId](const StockProduct& p) { return p.m_id == productId; });
	
	return (it != m_playerProducts.end()) ? it->m_quantity : 0;
}

/// @brief Get total value of all products in inventory
/// @return Total inventory value based on current prices
uint32_t Inventory::GetTotalInventoryValue() const
{
	uint32_t totalValue = 0;
	for (const auto& product : m_playerProducts)
	{
		totalValue += product.m_quantity * product.m_currentPrice;
	}
	return totalValue;
}

/// @brief Get current inventory volume
/// @return Current inventory volume used
float Inventory::GetCurrentInventoryVolume() const
{
	return m_currentInventoryVolume;
}

/// @brief Get maximum inventory volume capacity
/// @return Maximum inventory volume allowed
float Inventory::GetMaxInventoryVolume() const
{
	return s_maxInventoryVolume;
}

/// @brief Get reference to player products for read-only access
/// @return Const reference to player products vector
const std::vector<StockProduct>& Inventory::GetPlayerProducts() const
{
	return m_playerProducts;
}

// === Product Management ===

/// @brief Add quantity to a product in inventory and update volume
/// @param productId ID of the product to add to
/// @param quantity Amount to add
void Inventory::AddProduct(const std::string& productId, uint32_t quantity)
{
	if (quantity == 0)
	{
		return; // Nothing to add
	}

	// Find product in inventory
	StockProduct* product = FindProduct(productId);
	if (product == nullptr)
	{
		DebugLog("AddProduct - Product with ID: " + productId + " not found in inventory", DebugType::Warning);
		return;
	}

	// Add quantity
	product->m_quantity += quantity;

	// Update inventory volume
	float volumeAdded = quantity * product->m_volume;
	m_currentInventoryVolume += volumeAdded;

	DebugLog("AddProduct - " + product->m_name + " (ID: " + productId + ") - " +
		"Added: " + std::to_string(quantity) + 
		", New quantity: " + std::to_string(product->m_quantity) +
		", Volume added: " + std::to_string(volumeAdded) +
		", Total volume: " + std::to_string(m_currentInventoryVolume) + "/" + std::to_string(s_maxInventoryVolume));
}

/// @brief Remove quantity from a product in inventory and update volume
/// @param productId ID of the product to remove from
/// @param quantity Amount to remove
void Inventory::RemoveProduct(const std::string& productId, uint32_t quantity)
{
	if (quantity == 0)
	{
		return; // Nothing to remove
	}

	// Find product in inventory
	StockProduct* product = FindProduct(productId);
	if (product == nullptr)
	{
		DebugLog("RemoveProduct - Product with ID: " + productId + " not found in inventory", DebugType::Warning);
		return;
	}

	// Check if we have enough quantity to remove
	if (product->m_quantity < quantity)
	{
		DebugLog("RemoveProduct - Not enough quantity for " + product->m_name + " (ID: " + productId + ") - " +
			"Requested: " + std::to_string(quantity) + ", Available: " + std::to_string(product->m_quantity), DebugType::Warning);
		return;
	}

	// Remove quantity
	product->m_quantity -= quantity;

	// Update inventory volume
	float volumeRemoved = quantity * product->m_volume;
	m_currentInventoryVolume = std::max(0.0f, m_currentInventoryVolume - volumeRemoved);

	DebugLog("RemoveProduct - " + product->m_name + " (ID: " + productId + ") - " +
		"Removed: " + std::to_string(quantity) + 
		", New quantity: " + std::to_string(product->m_quantity) +
		", Volume removed: " + std::to_string(volumeRemoved) +
		", Total volume: " + std::to_string(m_currentInventoryVolume) + "/" + std::to_string(s_maxInventoryVolume));
}

// === Private Helper Functions ===

/// @brief Find product in inventory by ID
/// @param productId ID of the product to find
/// @return Pointer to product if found, nullptr otherwise
StockProduct* Inventory::FindProduct(const std::string& productId)
{
	auto it = std::find_if(m_playerProducts.begin(), m_playerProducts.end(),
		[&productId](const StockProduct& p) { return p.m_id == productId; });
	
	return (it != m_playerProducts.end()) ? &(*it) : nullptr;
}

	
