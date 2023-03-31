#include "StockDataLoadRunable.h"
#include "StockData.h"

StockDataLoadRunable::StockDataLoadRunable(StockData *stockdata):pStockData(stockdata)
{

}


StockDataLoadRunable::~StockDataLoadRunable()
{

}


void StockDataLoadRunable::run()
{
   pStockData->Init();
   pStockData->ReadAllStockDataFromStore();
}

