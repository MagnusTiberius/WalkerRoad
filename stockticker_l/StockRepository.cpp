#include "stdafx.h"
#include "StockRepository.h"


StockRepository::StockRepository()
{
}


StockRepository::~StockRepository()
{
}


void StockRepository::Start()
{
	stockListing = new StockDef::STOCKLISTING();
	stockListing->stockList = new map<string, StockDef::LP_STOCKPRICE>();
}

void StockRepository::CreateStock(char* code, char* name, char* price)
{
	StockDef::LP_STOCK stock = new StockDef::STOCK();
	stock->code = code;
	stock->name = name;

	StockDef::LP_STOCKPRICE stockPrice = new StockDef::STOCKPRICE();
	stockPrice->stock = stock;
	stockPrice->currentPrice.assign(price);
	stockPrice->subscriberList = new StockDef::SUBSCRIBERLIST();

	StockDef::STOCKLISTING &list = *stockListing;
	string stockname = "";
	map<string, StockDef::LP_STOCKPRICE> &_stockprice = *list.stockList;
	stockname.assign(stock->code);
	_stockprice[stockname] = stockPrice;
}


StockDef::LP_STOCKLISTING StockRepository::GetListing()
{
	return stockListing;
}