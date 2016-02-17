#pragma once
#include "stdafx.h"
#include "StocksDefinition.h"

class StockRepository
{
public:
	StockRepository();
	~StockRepository();

	void Start();
	void CreateStock(char* code, char* name, char* price);
	StockDef::LP_STOCKLISTING GetListing();

private:
	StockDef::LP_STOCKLISTING stockListing;

};

