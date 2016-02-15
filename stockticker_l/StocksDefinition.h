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
		char* name;
		SOCKET _socket;
	} SUBSCRIBER, *LP_SUBSCRIBER;

	typedef struct StockPrice
	{
		LP_STOCK stock;
		string currentPrice;
		map<string, LP_SUBSCRIBER>* subscriberList;
	} STOCKPRICE, *LP_STOCKPRICE;

	typedef struct StockListing 
	{
		map<string, LP_STOCKPRICE>* stockList;
	} STOCKLISTING, *LP_STOCKLISTING;


public:
	static LP_STOCKLISTING Init();
};