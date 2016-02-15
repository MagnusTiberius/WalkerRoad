#include "stdafx.h"
#include "StocksDefinition.h"

StockDef::StockDef()
{


}


StockDef::~StockDef()
{
}

StockDef::LP_STOCKLISTING StockDef::Init()
{
	LP_STOCKLISTING listing = new STOCKLISTING();

	LP_STOCK stock = new STOCK();
	stock->code = "ABT";
	stock->name = "Abbott Laboratories";

	LP_STOCKPRICE stockprice = new STOCKPRICE();
	stockprice->stock = stock;
	stockprice->currentPrice = "37.13";
	stockprice->subscriberList = new map<string, LP_SUBSCRIBER>();

	LP_STOCKLISTING stockListing = new STOCKLISTING();
	stockListing->stockList = new map<string, LP_STOCKPRICE>();

	map<string, LP_STOCKPRICE> &sl = *stockListing->stockList;
	string sStock(stockprice->stock->code);
	sl[sStock] = stockprice;

	// ==========
	stock = new STOCK();
	stock->code = "TSLA";
	stock->name = "Tesla Motors";

	stockprice = new STOCKPRICE();
	stockprice->stock = stock;
	stockprice->currentPrice = "151.00";
	stockprice->subscriberList = new map<string, LP_SUBSCRIBER>();

	sStock.assign(stockprice->stock->code);
	sl[sStock] = stockprice;

	// ==========
	stock = new STOCK();
	stock->code = "TSLA";
	stock->name = "Tesla Motors";

	stockprice = new STOCKPRICE();
	stockprice->stock = stock;
	stockprice->currentPrice = "151.00";
	stockprice->subscriberList = new map<string, LP_SUBSCRIBER>();

	sStock.assign(stockprice->stock->code);
	sl[sStock] = stockprice;

	// ==========
	stock = new STOCK();
	stock->code = "INTC";
	stock->name = "Intel Corp";

	stockprice = new STOCKPRICE();
	stockprice->stock = stock;
	stockprice->currentPrice = "28.64";
	stockprice->subscriberList = new map<string, LP_SUBSCRIBER>();

	sStock.assign(stockprice->stock->code);
	sl[sStock] = stockprice;

	// ==========
	stock = new STOCK();
	stock->code = "NKE";
	stock->name = "Nike Inc";

	stockprice = new STOCKPRICE();
	stockprice->stock = stock;
	stockprice->currentPrice = "56.42";
	stockprice->subscriberList = new map<string, LP_SUBSCRIBER>();

	sStock.assign(stockprice->stock->code);
	sl[sStock] = stockprice;

	return stockListing;
};