#pragma once
#include "stdafx.h"




class StockDef
{
public:
	StockDef();
	~StockDef();

public:
	typedef struct Stock  
	{
		char* code;
		char* name;
	} STOCK, *LP_STOCK;

	typedef struct Subscriber 
	{
		string name;
		SOCKET _socket;
		bool isValid;
	} SUBSCRIBER, *LP_SUBSCRIBER;

	typedef map<string, LP_SUBSCRIBER> SUBSCRIBERLIST, *LP_SUBSCRIBERLIST;

	typedef struct StockPrice
	{
		LP_STOCK stock;
		string currentPrice;
		map<string, LP_SUBSCRIBER>* subscriberList;
	} STOCKPRICE, *LP_STOCKPRICE;

	typedef map<string, LP_STOCKPRICE> STOCKLIST, *LP_STOCKLIST;

	typedef struct StockListing 
	{
		map<string, LP_STOCKPRICE>* stockList;
	} STOCKLISTING, *LP_STOCKLISTING;


public:
	static LP_STOCKLISTING Init();

private:
	LP_STOCKLISTING stockListing;
};