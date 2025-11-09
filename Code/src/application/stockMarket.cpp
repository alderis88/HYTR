#include "pch.h"
#include "stockMarket.h"
#include "utilTools.h"
#include "application.h"
#include "applicationUI.h"

/// @brief Initialize the entire stock market system
/// Loads all data files and sets up initial market state
/// @param app Reference to the main application instance
void StockMarket::InitializeStockMarket(Application* app)
{
	// Store application reference
	m_application = app;
	DebugLog("StockMarket - Application reference set");

	// Reset market timing
	m_currentCycleTime = 4.5f;
	m_cycleCount = 0;

	// Load stock products from JSON
	std::ostringstream pathBuilder;
	pathBuilder << Application::s_dataPath << "item_products.json";
	LoadJsonStockProducts(pathBuilder.str());

	// Load stock vendors from JSON
	pathBuilder.str("");  // Clear the stringstream
	pathBuilder.clear();  // Reset any error flags
	pathBuilder << Application::s_dataPath << "vendor_characters.json";
	LoadJsonStockVendors(pathBuilder.str());

	// Load news from JSON
	pathBuilder.str("");  // Clear the stringstream
	pathBuilder.clear();  // Reset any error flags
	pathBuilder << Application::s_dataPath << "news.json";
	LoadJsonNews(pathBuilder.str());

	// Initialize random starting values for all products
	InitializeProductValues();

	DebugLog("Stock Market initialization completed");
	//DebugLog(m_stockProducts[3].m_name);

	//Randomize starting prices for 5 stock products
	//Set the price text color and arrows based on start price (randomize change direction)

}

/// @brief Main update loop for the stock market system
/// Called every frame to handle timing and trigger market cycles
/// @param delta Time elapsed since last frame
void StockMarket::StockMarketUpdate(sf::Time delta)
{
	// Only update cycle timer if not in trade pause
	if (m_application && !m_application->m_inTradePause)
	{
		// Accumulate time for market cycle timing
		m_currentCycleTime += delta.asSeconds() /* * Application::s_stockGameSpeed */;

		// Check if it's time for a new market cycle
		if (m_currentCycleTime >= s_stockCycleTime)
		{
			// Execute market cycle: price updates, stock replenishment, trend shifts
			m_currentCycleTime = 0.0f;  // Reset cycle timer
			m_cycleCount++;             // Increment cycle counter
			StockMarketCycleStep();     // Perform all market updates
		}
	}

	// Always Update the cycle timer, even if no cycle step occurred
	CycleTimerUpdate();

}

/// @brief Execute one complete market cycle
/// Updates trends, reduces player impact, replenishes stock, and recalculates prices
void StockMarket::StockMarketCycleStep()
{
	DebugLog("Market Cycle #" + std::to_string(m_cycleCount) + " executing");

	// Update trend pointers for all stock products
	for (auto& product : m_stockProducts)
	{
		product.m_trendPointer++;
		if (product.m_trendPointer > 49)
		{
			product.m_trendPointer = 0;
		}

		// Reduce player impact over time
		ReducePlayerImpact(product);

		// Replenish stock
		ProductStockReplenishment(product);

		// Calculate and update current price
		CalculateProductPrice(product);
	}

	// Update UI displays with new product data
	if (m_application && m_application->GetApplicationUI())
	{
		m_application->GetApplicationUI()->UpdateProductDisplays();
	}
}/// @brief Update cycle timer (mainly for debugging purposes)
/// Currently disabled but can be used to monitor cycle timing
void StockMarket::CycleTimerUpdate()
{
	//DebugLog("CycleTime:" + std::to_string(m_currentCycleTime));
}

/// @brief Load stock products from JSON file
/// Parses item_products.json and populates the m_stockProducts vector
/// @param path Full path to the JSON file to load
void StockMarket::LoadJsonStockProducts(const std::string& path)
{
	DebugLog("Loading Stock Products from: " + path);
	std::ifstream stream(path);
	if (!stream.is_open()) {
		DebugLog("Failed to open file: " + path);
	}
	std::string fileData((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	DebugLog("File data size: " + std::to_string(fileData.size()));

	Json::Document document;
	document.Parse(fileData.c_str());
	if (document.HasParseError()) {
		DebugLog("JSON Parse Error: " + std::string(Json::GetParseError_En(document.GetParseError())));
		DebugLog("JSON Parse Error Offset: " + std::to_string(document.GetErrorOffset()));
		DebugLog("File size: " + std::to_string(fileData.size()));
		if (!fileData.empty()) {
			DebugLog("First 100 chars: " + fileData.substr(0, std::min(100, (int)fileData.size())));
		}
		DebugLog("Skipping JSON loading due to parse error");
		return; // Return early instead of asserting
	}

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

		//volume
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
		for (Json::SizeType i = 0; i < internalArrayObject.Size(); i++)
		{
			assert(internalArrayObject[i].IsFloat());
			newProduct.m_trends.push_back(internalArrayObject[i].GetFloat());
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

		m_stockProducts.push_back(std::move(newProduct));
	}
}

/// @brief Initialize random starting values for all loaded products
/// Sets random quantities, trend pointers, player impact, and calculates initial prices
void StockMarket::InitializeProductValues()
{
	// Create uniform distribution for trend pointer (0-49)
	std::uniform_int_distribution<uint32_t> trendDistribution(0, 49);

	// Create uniform distribution for random boolean (trend increased)
	std::uniform_int_distribution<int> boolDistribution(0, 1);

	// Iterate through all stock products
	for (auto& product : m_stockProducts)
	{
		// Create uniform distribution between 0 and m_maxQuantity (inclusive)
		std::uniform_int_distribution<uint32_t> quantityDistribution(0, product.m_maxQuantity);

		// Set random quantity
		product.m_quantity = quantityDistribution(Application::GetRandomGenerator());

		// Set random trend pointer between 0 and 49
		product.m_trendPointer = trendDistribution(Application::GetRandomGenerator());

		// Initialize current player impact to 0
		product.m_currentPlayerImpact = 0.0f;

		// Set random trend increased flag
		product.m_trendIncreased = boolDistribution(Application::GetRandomGenerator()) == 1;

		DebugLog("Product: " + product.m_name + " - Random quantity: " + std::to_string(product.m_quantity) + "/" + std::to_string(product.m_maxQuantity) + ", Trend pointer: " + std::to_string(product.m_trendPointer) + ", Initial price calculated");

		// Calculate initial price
		CalculateProductPrice(product);
	}
}

/// @brief Calculate and update the current price for a product
/// Uses trend data, random influence, and player impact to determine final price
/// @param product Reference to the product to update
void StockMarket::CalculateProductPrice(StockProduct& product)
{
	// Get the trend value from m_trends using m_trendPointer as index
	float trendValue = 0.0f;
	if (!product.m_trends.empty() && product.m_trendPointer < product.m_trends.size())
	{
		trendValue = product.m_trends[product.m_trendPointer];
	}

	// Calculate marketTrendPrice by remapping trendValue between minPrice and maxPrice
	// Assuming trendValue is normalized between 0 and 1, remap to [minPrice, maxPrice]
	float baseTrendPrice = product.m_minPrice + trendValue * (product.m_maxPrice - product.m_minPrice);

	// Apply random price influence factor
	std::uniform_real_distribution<float> randomInfluence(1.0f - s_randomPriceInfluenceFactor, 1.0f + s_randomPriceInfluenceFactor);
	float randomInfluenceFactor = randomInfluence(Application::GetRandomGenerator());


	// Apply player impact multiplier (reduced to 1/5th of original effect)
	float playerImpactMultiplier = 1.0f;
	float reducedPlayerImpact = product.m_currentPlayerImpact * 0.2f; // Reduce impact to 1/5th
	if (reducedPlayerImpact > 0.0f)
	{
		// Positive impact: 1 - reducedPlayerImpact, but minimum 0.9 (less extreme than before)
		playerImpactMultiplier = std::max(0.9f, 1.0f - reducedPlayerImpact);
	}
	else if (reducedPlayerImpact < 0.0f)
	{
		// Negative impact: 1 + |reducedPlayerImpact|, but maximum 1.1 (less extreme than before)
		playerImpactMultiplier = std::min(1.1f, 1.0f + std::abs(reducedPlayerImpact));
	}


	// Calculate and store price without player impact (ensure minimum value of 1)
	product.m_currentPriceWithoutPlayerImpact = std::max(1u, static_cast<uint32_t>(product.m_basePrice * baseTrendPrice * randomInfluenceFactor));

	// Calculate current price: basePrice * aggregated multiplier (ensure minimum value of 1)
	uint32_t newPrice = std::max(1u, static_cast<uint32_t>(playerImpactMultiplier * product.m_currentPriceWithoutPlayerImpact));

	// Compare with current price and set trend direction
	if (newPrice > product.m_currentPrice)
	{
		product.m_trendIncreased = true;
	}
	else if (newPrice < product.m_currentPrice)
	{
		product.m_trendIncreased = false;
	}
	// If prices are equal, keep the previous trend state

	// Update the current price
	product.m_currentPrice = newPrice;

	// Debug log the price calculation
	// DebugLog("Product: " + product.m_name + " - TV at " + std::to_string(product.m_trendPointer) + ": " + std::to_string(trendValue) +
	// 	", BTP: " + std::to_string(baseTrendPrice) +
	// 	", RIF: " + std::to_string(randomInfluenceFactor) +
	// 	", CPI: " + std::to_string(product.m_currentPlayerImpact) +
	// 	", PIM: " + std::to_string(playerImpactMultiplier) +
	// 	", Final Price: " + std::to_string(product.m_currentPrice) +
	// 	", Trend Increased: " + (product.m_trendIncreased ? "true" : "false"));

	}

/// @brief Calculate price using only player influence change
/// Takes m_currentPriceWithoutPlayerImpact and applies only player impact multiplier and trend calculation
/// @param product Reference to the product to update
void StockMarket::CalculateOnlyPlayerInfluenceChangePrice(StockProduct& product)
{
	// Apply player impact multiplier
	float playerImpactMultiplier = 1.0f;
	if (product.m_currentPlayerImpact > 0.0f)
	{
		// Positive impact: 1 - m_currentPlayerImpact, but minimum 0.5
		playerImpactMultiplier = std::max(0.5f, 1.0f - product.m_currentPlayerImpact);
	}
	else if (product.m_currentPlayerImpact < 0.0f)
	{
		// Negative impact: 1 + |m_currentPlayerImpact|, but maximum 1.5
		playerImpactMultiplier = std::min(1.5f, 1.0f + std::abs(product.m_currentPlayerImpact));
	}

	// Calculate new price using the stored price without player impact (ensure minimum value of 1)
	uint32_t newPrice = std::max(1u, static_cast<uint32_t>(playerImpactMultiplier * product.m_currentPriceWithoutPlayerImpact));

	// Compare with current price and set trend direction
	if (newPrice > product.m_currentPrice)
	{
		product.m_trendIncreased = true;
	}
	else if (newPrice < product.m_currentPrice)
	{
		product.m_trendIncreased = false;
	}
	// If prices are equal, keep the previous trend state

	// Update the current price
	product.m_currentPrice = newPrice;

	// Debug log the price calculation
	// DebugLog("CalculateOnlyPlayerInfluenceChangePrice - Product: " + product.m_name +
	// 	" - CPI: " + std::to_string(product.m_currentPlayerImpact) +
	// 	", PIM: " + std::to_string(playerImpactMultiplier) +
	// 	", Base Price Without Impact: " + std::to_string(product.m_currentPriceWithoutPlayerImpact) +
	// 	", Final Price: " + std::to_string(product.m_currentPrice) +
	// 	", Trend Increased: " + (product.m_trendIncreased ? "true" : "false"));

	if (m_application && m_application->GetApplicationUI())
	{
		m_application->GetApplicationUI()->UpdateProductDisplays();
	}
}

/// @brief Reduce player impact on product price over time
/// Gradually moves player impact back toward 0 (neutral)
/// @param product Reference to the product to update
void StockMarket::ReducePlayerImpact(StockProduct& product)
{
	if (product.m_currentPlayerImpact != 0.0f)
	{
		float oldImpact = product.m_currentPlayerImpact;

		if (product.m_currentPlayerImpact > 0.0f)
		{
			// Positive impact: reduce by m_playerImpact, but minimum 0
			product.m_currentPlayerImpact = std::max(0.0f, product.m_currentPlayerImpact - product.m_playerImpact);
		}
		else if (product.m_currentPlayerImpact < 0.0f)
		{
			// Negative impact: increase (reduce magnitude) by m_playerImpact, but maximum 0
			product.m_currentPlayerImpact = std::min(0.0f, product.m_currentPlayerImpact + product.m_playerImpact);
		}

		// Debug log if there was a change
		if (oldImpact != product.m_currentPlayerImpact)
		{
			DebugLog("Product: " + product.m_name + " - Player impact reduced from " +
				std::to_string(oldImpact) + " to " + std::to_string(product.m_currentPlayerImpact));
		}
	}
}

/// @brief Replenish stock quantity for a product
/// Adds random amount based on stackReplenishment value (75%-125% variation)
/// @param product Reference to the product to replenish
void StockMarket::ProductStockReplenishment(StockProduct& product)
{
	// Generate random number between 0.75 and 1.25
	std::uniform_real_distribution<float> replenishmentMultiplier(0.75f, 1.25f);
	float randomMultiplier = replenishmentMultiplier(Application::GetRandomGenerator());

	// Calculate replenishment amount
	uint32_t replenishmentAmount = static_cast<uint32_t>(product.m_stackReplenishment * randomMultiplier);

	// Store old quantity for logging
	uint32_t oldQuantity = product.m_quantity;

	// Add replenishment to current quantity, but don't exceed max quantity
	product.m_quantity = std::min(product.m_maxQuantity, product.m_quantity + replenishmentAmount);

	// Debug log the replenishment
	if (replenishmentAmount > 0)
	{
		DebugLog("Product: " + product.m_name + " - Stock replenished: +" + std::to_string(replenishmentAmount) +
			" (multiplier: " + std::to_string(randomMultiplier) + "), " +
			"Quantity: " + std::to_string(oldQuantity) + " -> " + std::to_string(product.m_quantity) +
			"/" + std::to_string(product.m_maxQuantity));
	}
}

/// @brief Find a stock product by its unique ID
/// @param productId The product ID to search for
/// @return Pointer to the product if found, nullptr if not found
StockProduct* StockMarket::GetStockProductById(const std::string& productId)
{
	// Find the product by ID
	for (auto& product : m_stockProducts)
	{
		if (product.m_id == productId)
		{
			return &product;
		}
	}

	// Product not found
	return nullptr;
}

/// @brief Get vendor by associated product ID
/// @param productId ID of the product to find the associated vendor for
/// @return Pointer to StockVendor if found, nullptr otherwise
StockVendor* StockMarket::GetStockVendorByProductId(const std::string& productId)
{
	// Find the vendor by product ID
	for (auto& vendor : m_stockVendors)
	{
		if (vendor.m_productId == productId)
		{
			return &vendor;
		}
	}

	// Vendor not found
	return nullptr;
}

/// @brief Validate if a buy transaction is possible
/// Checks if sufficient stock is available, player has enough money, and inventory has space
/// @param productId ID of the product to check
/// @param desiredQuantity Amount the player wants to buy
/// @return true if transaction is valid, false if insufficient stock, money, or inventory space
bool StockMarket::ValidateBuyFromStock(const std::string& productId, uint32_t desiredQuantity)
{
	// Get the product by ID
	StockProduct* product = GetStockProductById(productId);

	if (product == nullptr)
	{
		// Product not found
		DebugLog("ValidateBuyFromStock - Product with ID: " + productId + " not found in stock market", DebugType::Warning);
		return false;
	}

	// Check if desired quantity is available in stock
	if (product->m_quantity < desiredQuantity)
	{
		DebugLog("ValidateBuyFromStock - Not enough stock for product: " + product->m_name +
			" (ID: " + productId + ") - Requested: " + std::to_string(desiredQuantity) +
			", Available: " + std::to_string(product->m_quantity), DebugType::Warning);
		return false;
	}

	// Check if application and inventory references are available
	if (!m_application || !m_application->GetPlayerInventory())
	{
		DebugLog("ValidateBuyFromStock - No application or inventory reference available", DebugType::Warning);
		return false;
	}

	// Calculate total cost
	uint32_t totalCost = desiredQuantity * product->m_currentPrice;

	// Check if player has enough money
	uint32_t currentMoney = m_application->GetPlayerInventory()->GetCurrentMoney();
	if (currentMoney < totalCost)
	{
		DebugLog("ValidateBuyFromStock - Not enough money for product: " + product->m_name +
			" (ID: " + productId + ") - Cost: " + std::to_string(totalCost) +
			", Available money: " + std::to_string(currentMoney), DebugType::Warning);
		return false;
	}

	// Check if inventory has enough space (volume-based)
	float requiredVolume = desiredQuantity * product->m_volume;
	float currentVolume = m_application->GetPlayerInventory()->GetCurrentInventoryVolume();
	float maxVolume = m_application->GetPlayerInventory()->GetMaxInventoryVolume();
	float availableSpace = maxVolume - currentVolume;

	if (requiredVolume > availableSpace)
	{
		DebugLog("ValidateBuyFromStock - Not enough inventory space for product: " + product->m_name +
			" (ID: " + productId + ") - Required: " + std::to_string(requiredVolume) +
			", Available space: " + std::to_string(availableSpace), DebugType::Warning);
		return false;
	}

	// All checks passed - transaction is valid
	DebugLog("ValidateBuyFromStock - Product: " + product->m_name + " (ID: " + productId + ") - " +
		"Quantity: " + std::to_string(desiredQuantity) +
		", Cost: " + std::to_string(totalCost) +
		", Volume required: " + std::to_string(requiredVolume) +
		" - Transaction VALID");

	return true;
}

/// @brief Validate if a sell transaction is possible
/// Checks if the player has the specified quantity of the product in their inventory
/// @param productId ID of the product to check
/// @param desiredQuantity Amount the player wants to sell
/// @return true if transaction is valid, false if insufficient quantity in inventory
bool StockMarket::ValidateSellForStock(const std::string& productId, uint32_t desiredQuantity)
{
	// Get the product by ID to verify it exists
	StockProduct* product = GetStockProductById(productId);

	if (product == nullptr)
	{
		// Product not found in stock market
		DebugLog("ValidateSellForStock - Product with ID: " + productId + " not found in stock market", DebugType::Warning);
		return false;
	}

	// Check if application and inventory references are available
	if (!m_application || !m_application->GetPlayerInventory())
	{
		DebugLog("ValidateSellForStock - No application or inventory reference available", DebugType::Warning);
		return false;
	}

	// Get player's current quantity of this product in inventory
	uint32_t playerQuantity = m_application->GetPlayerInventory()->GetProductQuantity(productId);

	// Check if player has enough quantity to sell
	if (playerQuantity < desiredQuantity)
	{
		DebugLog("ValidateSellForStock - Not enough quantity in inventory for product: " + product->m_name +
			" (ID: " + productId + ") - Requested to sell: " + std::to_string(desiredQuantity) +
			", Available in inventory: " + std::to_string(playerQuantity), DebugType::Warning);
		return false;
	}

	// All checks passed - transaction is valid
	DebugLog("ValidateSellForStock - Product: " + product->m_name + " (ID: " + productId + ") - " +
		"Quantity to sell: " + std::to_string(desiredQuantity) +
		", Available in inventory: " + std::to_string(playerQuantity) +
		" - Transaction VALID");

	return true;
}

/// @brief Execute a buy transaction from stock market
/// Reduces the available stock quantity by the exact amount purchased
/// @param productId ID of the product to buy
/// @param quantity Amount to purchase
/// @return true if successful, false if insufficient stock or product not found
bool StockMarket::BuyFromStock(const std::string& productId, uint32_t quantity)
{
	// Use validation function to check all prerequisites (stock, money, inventory space)
	if (!ValidateBuyFromStock(productId, quantity))
	{
		// ValidateBuyFromStock already logs specific error messages
		return false;
	}

	// Get the product (we know it exists because validation passed)
	StockProduct* product = GetStockProductById(productId);

	// Store old quantity for logging
	uint32_t oldQuantity = product->m_quantity;

	// Calculate total cost (quantity * current price)
	uint32_t totalCost = quantity * product->m_currentPrice;

	// Execute the transaction (we know all conditions are met)
	uint32_t currentMoney = m_application->GetPlayerInventory()->GetCurrentMoney();

	// Deduct the cost from player's money
	m_application->GetPlayerInventory()->SetCurrentMoney(currentMoney - totalCost);

	// Add the purchased product to player's inventory
	m_application->GetPlayerInventory()->AddProduct(productId, quantity);

	// Reduce stock quantity exactly by the requested amount
	product->m_quantity -= quantity;

	// Update current player impact for purchase (increases demand pressure) - REVERSED EFFECT
	product->m_currentPlayerImpact += product->m_playerImpact * quantity;

	// Clamp current player impact between -0.5 and 0.5
	product->m_currentPlayerImpact = std::max(-0.5f, std::min(0.5f, product->m_currentPlayerImpact));

	// Debug log the purchase
	DebugLog("BuyFromStock - Product: " + product->m_name + " (ID: " + productId + ") - " +
		"Bought: " + std::to_string(quantity) +
		", Cost: " + std::to_string(totalCost) + " (price: " + std::to_string(product->m_currentPrice) + ")" +
		", Quantity: " + std::to_string(oldQuantity) + " -> " + std::to_string(product->m_quantity) +
		"/" + std::to_string(product->m_maxQuantity));

	// Update price based on player impact after purchase
	CalculateOnlyPlayerInfluenceChangePrice(*product);

	// Update money display and inventory buttons in UI
	if (m_application && m_application->GetApplicationUI())
	{
		m_application->GetApplicationUI()->UpdateCurrentMoneyDisplay();
		m_application->GetApplicationUI()->UpdateInventoryVerticalButtons();
	}

	return true;
}

/// @brief Execute a sell transaction to stock market
/// Increases available stock by quantity * sellStackRatio (not 1:1 ratio)
/// @param productId ID of the product to sell
/// @param quantity Amount player is selling
/// @return true if successful, false if insufficient quantity in inventory or product not found
bool StockMarket::SellForStock(const std::string& productId, uint32_t quantity)
{
	// Use validation function to check if player has enough quantity in inventory
	if (!ValidateSellForStock(productId, quantity))
	{
		// ValidateSellForStock already logs specific error messages
		return false;
	}

	// Get the product (we know it exists because validation passed)
	StockProduct* product = GetStockProductById(productId);

	// Store old quantity for logging
	uint32_t oldQuantity = product->m_quantity;

	// Calculate total earnings (quantity * current price)
	uint32_t totalEarnings = quantity * product->m_currentPrice;

	// Execute the transaction (we know all conditions are met)
	uint32_t currentMoney = m_application->GetPlayerInventory()->GetCurrentMoney();

	// Add the earnings to player's money
	m_application->GetPlayerInventory()->SetCurrentMoney(currentMoney + totalEarnings);

	// Remove the sold products from player inventory
	m_application->GetPlayerInventory()->RemoveProduct(productId, quantity);

	// Calculate the actual stock increase using sellStackRatio
	uint32_t stockIncrease = static_cast<uint32_t>(quantity * product->m_sellStackRatio);

	// Ensure we don't exceed max quantity
	product->m_quantity = std::min(product->m_maxQuantity, product->m_quantity + stockIncrease);

	// Update current player impact for sale (decreases demand pressure) - REVERSED EFFECT
	product->m_currentPlayerImpact -= product->m_playerImpact * quantity;

	// Clamp current player impact between -0.5 and 0.5
	product->m_currentPlayerImpact = std::max(-0.5f, std::min(0.5f, product->m_currentPlayerImpact));

	// Debug log the sale
	DebugLog("SellForStock - Product: " + product->m_name + " (ID: " + productId + ") - " +
		"Sold: " + std::to_string(quantity) +
		", Earnings: " + std::to_string(totalEarnings) + " (price: " + std::to_string(product->m_currentPrice) + ")" +
		", Stock increase: " + std::to_string(stockIncrease) +
		" (ratio: " + std::to_string(product->m_sellStackRatio) + "), " +
		"Quantity: " + std::to_string(oldQuantity) + " -> " + std::to_string(product->m_quantity) +
		"/" + std::to_string(product->m_maxQuantity));

	// Update price based on player impact after sale
	CalculateOnlyPlayerInfluenceChangePrice(*product);

	// Update money display and inventory buttons in UI
	if (m_application && m_application->GetApplicationUI())
	{
		m_application->GetApplicationUI()->UpdateCurrentMoneyDisplay();
		m_application->GetApplicationUI()->UpdateInventoryVerticalButtons();
	}

	return true;
}

/// @brief Load vendor characters from JSON file
/// Parses vendor_characters.json and populates the m_stockVendors vector
/// @param path Full path to the JSON file to load
void StockMarket::LoadJsonStockVendors(const std::string& path)
{
	DebugLog("Loading Stock Vendors from: " + path);
	std::ifstream stream(path);

	if (!stream.is_open()) {
		DebugLog("ERROR: Could not open file: " + path);
		return; // Gracefully handle the error instead of assertion
	}

	std::string fileData((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	if (fileData.empty()) {
		DebugLog("ERROR: File is empty: " + path);
		return; // Gracefully handle the error instead of assertion
	}

	DebugLog("File size: " + std::to_string(fileData.size()) + " bytes");

	Json::Document document;
	document.Parse(fileData.c_str());

	if (document.HasParseError()) {
		DebugLog("ERROR: JSON Parse Error at offset " + std::to_string(document.GetErrorOffset()));
		DebugLog("ERROR: " + std::string(Json::GetParseError_En(document.GetParseError())));
		DebugLog("File content preview (first 200 chars): " + fileData.substr(0, 200));
		return; // Gracefully handle the error instead of assertion
	}

	if (!document.IsObject()) {
		DebugLog("ERROR: JSON document is not an object: " + path);
		return;
	}

	//characters
	if (!document.HasMember("characters")) {
		DebugLog("ERROR: JSON document missing 'characters' member: " + path);
		return;
	}
	const Json::Value& arrayObject = document["characters"];
	if (!arrayObject.IsArray()) {
		DebugLog("ERROR: 'characters' is not an array: " + path);
		return;
	}
	for (Json::SizeType i = 0; i < arrayObject.Size(); i++)
	{
		StockVendor newVendor;

		// id
		assert(arrayObject[i].HasMember("id"));
		assert(arrayObject[i]["id"].IsString());
		newVendor.m_id = arrayObject[i]["id"].GetString();

		// product_id
		assert(arrayObject[i].HasMember("product_id"));
		assert(arrayObject[i]["product_id"].IsString());
		newVendor.m_productId = arrayObject[i]["product_id"].GetString();

		// name
		assert(arrayObject[i].HasMember("name"));
		assert(arrayObject[i]["name"].IsString());
		newVendor.m_name = arrayObject[i]["name"].GetString();

		// alias (optional)
		if (arrayObject[i].HasMember("alias"))
		{
			assert(arrayObject[i]["alias"].IsString());
			newVendor.m_alias = arrayObject[i]["alias"].GetString();
		}

		// company
		assert(arrayObject[i].HasMember("company"));
		assert(arrayObject[i]["company"].IsString());
		newVendor.m_company = arrayObject[i]["company"].GetString();

		// role
		assert(arrayObject[i].HasMember("role"));
		assert(arrayObject[i]["role"].IsString());
		newVendor.m_role = arrayObject[i]["role"].GetString();

		// profile
		assert(arrayObject[i].HasMember("profile"));
		assert(arrayObject[i]["profile"].IsString());
		newVendor.m_profile = arrayObject[i]["profile"].GetString();

		// appearance
		assert(arrayObject[i].HasMember("appearance"));
		assert(arrayObject[i]["appearance"].IsString());
		newVendor.m_appearance = arrayObject[i]["appearance"].GetString();

		// mood
		assert(arrayObject[i].HasMember("mood"));
		assert(arrayObject[i]["mood"].IsString());
		newVendor.m_mood = arrayObject[i]["mood"].GetString();

		// colorTheme
		assert(arrayObject[i].HasMember("colorTheme"));
		const Json::Value& colorArray = arrayObject[i]["colorTheme"];
		assert(colorArray.IsArray());
		for (Json::SizeType j = 0; j < colorArray.Size(); j++)
		{
			assert(colorArray[j].IsString());
			newVendor.m_colorTheme.push_back(colorArray[j].GetString());
		}

		// quote
		assert(arrayObject[i].HasMember("quote"));
		assert(arrayObject[i]["quote"].IsString());
		newVendor.m_quote = arrayObject[i]["quote"].GetString();

		// style
		assert(arrayObject[i].HasMember("style"));
		assert(arrayObject[i]["style"].IsString());
		newVendor.m_style = arrayObject[i]["style"].GetString();

		// companyInfo
		assert(arrayObject[i].HasMember("companyInfo"));
		assert(arrayObject[i]["companyInfo"].IsString());
		newVendor.m_companyInfo = arrayObject[i]["companyInfo"].GetString();

		// personality
		assert(arrayObject[i].HasMember("personality"));
		const Json::Value& personalityObject = arrayObject[i]["personality"];
		assert(personalityObject.IsObject());

		assert(personalityObject.HasMember("discipline"));
		assert(personalityObject["discipline"].IsInt());
		newVendor.m_personality.discipline = static_cast<uint32_t>(personalityObject["discipline"].GetInt());

		assert(personalityObject.HasMember("riskTaking"));
		assert(personalityObject["riskTaking"].IsInt());
		newVendor.m_personality.riskTaking = static_cast<uint32_t>(personalityObject["riskTaking"].GetInt());

		assert(personalityObject.HasMember("greed"));
		assert(personalityObject["greed"].IsInt());
		newVendor.m_personality.greed = static_cast<uint32_t>(personalityObject["greed"].GetInt());

		assert(personalityObject.HasMember("honor"));
		assert(personalityObject["honor"].IsInt());
		newVendor.m_personality.honor = static_cast<uint32_t>(personalityObject["honor"].GetInt());

		m_stockVendors.push_back(std::move(newVendor));
	}

	DebugLog("Loaded " + std::to_string(m_stockVendors.size()) + " stock vendors");
}

/// @brief Load market news from JSON file
/// Parses news.json, shuffles items randomly, and resets news index
/// @param path Full path to the JSON file to load
void StockMarket::LoadJsonNews(const std::string& path)
{
	DebugLog("Loading News from: " + path);
	std::ifstream stream(path);
	std::string fileData((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	Json::Document document;
	document.Parse(fileData.c_str());
	assert(!document.HasParseError());

	assert(document.IsObject());

	//news
	assert(document.HasMember("news"));
	const Json::Value& arrayObject = document["news"];
	assert(arrayObject.IsArray());
	for (Json::SizeType i = 0; i < arrayObject.Size(); i++)
	{
		News newNews;

		// newsContent
		assert(arrayObject[i].IsString());
		newNews.m_newsContent = arrayObject[i].GetString();

		m_news.push_back(std::move(newNews));
	}

	// Shuffle the news items randomly
	std::shuffle(m_news.begin(), m_news.end(), Application::GetRandomGenerator());

	// Reset news index to 0
	m_newsIndex = 0;

	DebugLog("Loaded and shuffled " + std::to_string(m_news.size()) + " news items");
}

/// @brief Get the next news item in cyclical rotation
/// When all news have been shown, reshuffles and starts over
/// @return Pointer to next news item, nullptr if no news loaded
News* StockMarket::GetNextNews()
{
	// Safety check: ensure we have news loaded
	if (m_news.empty())
	{
		DebugLog("GetNextNews - No news items available", DebugType::Warning);
		return nullptr;
	}

	// Safety check: ensure index is valid before array access
	if (m_newsIndex >= m_news.size())
	{
		m_newsIndex = 0;
		DebugLog("GetNextNews - Index was out of bounds, reset to 0");
	}

	// Get the current news item (safe now)
	News* currentNews = &m_news[m_newsIndex];

	// Debug log before incrementing
	DebugLog("GetNextNews - Returning news at index " + std::to_string(m_newsIndex));

	// Increment the index for next call
	m_newsIndex++;

	// Check if next index would overflow, reset and shuffle if so
	if (m_newsIndex >= m_news.size())
	{
		m_newsIndex = 0;
		// Shuffle the news items randomly
		std::shuffle(m_news.begin(), m_news.end(), Application::GetRandomGenerator());
		DebugLog("GetNextNews - Reached end of news, reset to 0 and reshuffled for next call");
	}

	return currentNews;
}

/// @brief Set the current product ID for market operations
/// @param productId The ID of the product to select (e.g., "TRI", "NFX", etc.)
void StockMarket::SetCurrentProductID(const std::string& productId)
{
	currentProductID = productId;
	DebugLog("StockMarket - Current product ID set to: " + productId);
}
