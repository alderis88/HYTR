#include "pch.h"
#include "inventory.h"
#include "application.h"

Inventory::Inventory() = default;
Inventory::~Inventory() = default;


void Inventory::InventoryInitialize()
{
	//Starting money
	m_currentMoney = 10000;
}	
