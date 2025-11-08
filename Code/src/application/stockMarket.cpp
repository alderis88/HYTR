#include "pch.h"
#include "stockMarket.h"
#include "utilTools.h"
#include "application.h"

void StockMarket::InitializeStockMarket()
{
	currentCyleTime = 0.0f;
	cycleCount = 0;

	std::ostringstream pathBuilder;
	pathBuilder << Application::s_dataPath << "item_products.json";
	LoadStockProducts(pathBuilder.str());
	
	// Initialize random quantities for all stock products
	InitializeProductValues();

	DebugLog("Stock Init Done");
	//DebugLog(m_stockProducts[3].m_name);

	//Randomize starting prices for 5 stock products
	//Set the price text color and arrows based on start price (randomize change direction)

}

void StockMarket::StockMarketUpdate(sf::Time delta)
{
		currentCyleTime += delta.asSeconds() ;

		if (currentCyleTime >= stockCycleTime)
		{
			// Perform stock market ApplicationUpdate logic here
			// For example, adjust stock prices, notify players, etc.
			// Reset the cycle timer

			currentCyleTime = 0.0f;
			cycleCount++;
			StockMarketCycleStep();
		}

		// Always Update the cycle timer, even if no cycle step occurred
		CycleTimerUpdate();
}

void StockMarket::StockMarketCycleStep()
{
	DebugLog("Cycle:" + std::to_string(cycleCount));

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

	// refresh the 5 stock product to new price.

	// refresh the prize text color and arrows based on change
	// refresh the avalible stock amounts

}

void StockMarket::CycleTimerUpdate()
{
	//DebugLog("CycleTime:" + std::to_string(currentCyleTime));
}

void StockMarket::LoadStockProducts(const std::string& path)
{
	DebugLog("Loading Stock Products from: " + path);
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

		m_stockProducts.push_back(std::move(newProduct));
	}
}

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
	
	float aggregatedPriceMultiplier = baseTrendPrice * randomInfluenceFactor * playerImpactMultiplier;
	
	// Calculate current price: basePrice * aggregated multiplier
	uint32_t newPrice = static_cast<uint32_t>(product.m_basePrice * aggregatedPriceMultiplier);
	
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
	DebugLog("Product: " + product.m_name + " - TV at " + std::to_string(product.m_trendPointer) + ": " + std::to_string(trendValue) +
			 ", BTP: " + std::to_string(baseTrendPrice) + 
			 ", RIF: " + std::to_string(randomInfluenceFactor) + 
			 ", CPI: " + std::to_string(product.m_currentPlayerImpact) + 
			 ", PIM: " + std::to_string(playerImpactMultiplier) + 
			 ", APM: " + std::to_string(aggregatedPriceMultiplier) + 
			 ", Final Price: " + std::to_string(product.m_currentPrice) + 
			 ", Trend Increased: " + (product.m_trendIncreased ? "true" : "false"));
}

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
