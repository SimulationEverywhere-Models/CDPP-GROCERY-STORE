#ifndef __STORE_H
#define __STORE_H

#define  ARRIVAL_DISTRIBUTION "arrivalDistribution"
#define  SEARCH_TIME_DISTRIBUTION "searchTimeDistribution"
#define  TRAVEL_TIME_DISTRIBUTION "travelTimeDistribution"
#define  NUMBER_OF_ITEMS_DISTRIBUTION "numberOfItemsDistribution"
#define  CAR_OR_NOT "carOrNot"
#define  OK_GO "okGo"
#define  HOLD_OFF "holdOff"
#define  ONE_LESS_CUSTOMER "oneLessCustomer"
#define  OUTPUT_CAR_OR_NOT "outputCarOrNot"
#define  OUTPUT_WHICH_CASH "outputWhichCash"
#define  OUTPUT_SEARCH_TIME "outputSearchTime"
#define  OUTPUT_TRAVEL_TIME "outputTravelTime"
#define  OUTPUT_NUMBER_OF_ITEMS "outputNumberOfItems"

#define  ARRIVAL_AVERAGE "arrivalAverage"
#define  SEARCH_TIME_AVERAGE "searchTimeAverage"
#define  TRAVEL_TIME_AVERAGE "travelTimeAverage"
#define  TRAVEL_TIME_STANDARD_ERROR "travelTimeStandardError"
#define  NUMBER_OF_ITEMS_AVERAGE "numberOfItemsAverage"
#define  NUMBER_OF_ITEMS_STANDARD_ERROR "numberOfItemsStandardError"
#define  WHICH_CASH "whichCash"
#define  CUSTOMER_FINISHED "customerFinished"
#define	 BAG_ID "bagId"
#define	 CUSTOMER_ID "customerId"
#define  BAG_SEARCH_TIME "bagSearchTime"
#define  CUSTOMER_OUTPUT "customerOutput"
#define  REMOVE_BAG "removeBag"
#define  BAG_REMOVED "bagRemoved"

#define  MAXIMUM_NUMBER_OF_TRAVELERS 20
#define  MAXIMUM_NUMBER_OF_SEARCHERS 20
#define  MAXIMUM_NUMBER_OF_BAGS 20
#define  NUMBER_OF_CUSTOMERS 20

static const short EXPRESS_CASH_MAXIMUM=8;
static const short EXPRESS_CASH=3;
static const short MAX_QUEUE_SIZE=2;

#define NUM_QUEUES 4
#define TIME_PER_ITEM 1

typedef struct 
{
	Value searchTime;
	Value numberOfItems;
	Value carOrNot;
	Value travelTime;
	Value customerId;
}customerProperties;

typedef struct
{
	Time countdownFinish;
	short customerId;	
} Countdown;

#endif
