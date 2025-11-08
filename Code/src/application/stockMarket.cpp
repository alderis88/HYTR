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
	LoadJsonStockProducts(pathBuilder.str());
	
	// Initialize random quantities for all stock products
	InitializeProductValues();

	DebugLog("Stock Init Done");
	//DebugLog(m_stockProducts[3].m_name);

	//Randomize starting prices for 5 stock products
	//Set the price text color and arrows based on start price (randomize change direction)

}

void StockMarket::StockMarketUpdate(sf::Time delta)
{
	if (!Application::s_pauseGame)
	{
		currentCyleTime += delta.asSeconds() /* * Application::s_stockGameSpeed */;

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

void StockMarket::LoadJsonStockProducts(const std::string& path)
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

bool StockMarket::ValidateBuyFromStock(const std::string& productId, uint32_t desiredQuantity)
{
	// Get the product by ID
	StockProduct* product = GetStockProductById(productId);
	
	if (product == nullptr)
	{
		// Product not found
		DebugLog("Product with ID: " + productId + " not found in stock market", DebugType::Warning);
		return false;
	}
	
	// Check if desired quantity is available
	bool isValidTrade = product->m_quantity >= desiredQuantity;
	
	// Debug log the validation result
	DebugLog("Product: " + product->m_name + " (ID: " + productId + ") - " +
			 "Desired quantity: " + std::to_string(desiredQuantity) + 
			 ", Available: " + std::to_string(product->m_quantity) + 
			 ", Valid trade: " + (isValidTrade ? "true" : "false"));
	
	return isValidTrade;
}

bool StockMarket::BuyFromStock(const std::string& productId, uint32_t quantity)
{
	// Get the product by ID
	StockProduct* product = GetStockProductById(productId);
	
	if (product == nullptr)
	{
		// Product not found
		DebugLog("BuyFromStock - Product with ID: " + productId + " not found in stock market", DebugType::Warning);
		return false;
	}
	
	// Check if we have enough stock
	if (product->m_quantity < quantity)
	{
		DebugLog("BuyFromStock - Not enough stock for product: " + product->m_name + 
				 " (ID: " + productId + ") - Requested: " + std::to_string(quantity) + 
				 ", Available: " + std::to_string(product->m_quantity), DebugType::Warning);
		return false;
	}
	
	// Store old quantity for logging
	uint32_t oldQuantity = product->m_quantity;
	
	// Reduce stock quantity exactly by the requested amount
	product->m_quantity -= quantity;
	
	// Debug log the purchase
	DebugLog("BuyFromStock - Product: " + product->m_name + " (ID: " + productId + ") - " +
			 "Bought: " + std::to_string(quantity) + 
			 ", Quantity: " + std::to_string(oldQuantity) + " -> " + std::to_string(product->m_quantity) + 
			 "/" + std::to_string(product->m_maxQuantity));
	
	return true;
}

void StockMarket::SellForStock(const std::string& productId, uint32_t quantity)
{
	// Get the product by ID
	StockProduct* product = GetStockProductById(productId);
	
	if (product == nullptr)
	{
		// Product not found
		DebugLog("SellForStock - Product with ID: " + productId + " not found in stock market", DebugType::Warning);
		return;
	}
	
	// Store old quantity for logging
	uint32_t oldQuantity = product->m_quantity;
	
	// Calculate the actual stock increase using sellStackRatio
	uint32_t stockIncrease = static_cast<uint32_t>(quantity * product->m_sellStackRatio);
	
	// Ensure we don't exceed max quantity
	product->m_quantity = std::min(product->m_maxQuantity, product->m_quantity + stockIncrease);
	
	// Debug log the sale
	DebugLog("SellForStock - Product: " + product->m_name + " (ID: " + productId + ") - " +
			 "Sold: " + std::to_string(quantity) + 
			 ", Stock increase: " + std::to_string(stockIncrease) + 
			 " (ratio: " + std::to_string(product->m_sellStackRatio) + "), " +
			 "Quantity: " + std::to_string(oldQuantity) + " -> " + std::to_string(product->m_quantity) + 
			 "/" + std::to_string(product->m_maxQuantity));
}

void StockMarket::LoadJsonStockVendors(const std::string& path)
{
	DebugLog("Loading Stock Vendors from: " + path);
	std::ifstream stream(path);
	std::string fileData((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	Json::Document document;
	document.Parse(fileData.c_str());
	assert(!document.HasParseError());

	assert(document.IsObject());

	//characters
	assert(document.HasMember("characters"));
	const Json::Value& arrayObject = document["characters"];
	assert(arrayObject.IsArray());
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

News* StockMarket::GetNextNews()
{
	// Check if we have any news items
	if (m_news.empty())
	{
		DebugLog("GetNextNews - No news items available", DebugType::Warning);
		return nullptr;
	}
	
	// Ensure index is within bounds before accessing
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
