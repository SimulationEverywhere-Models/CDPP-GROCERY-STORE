/*******************************************************************
*
*  DESCRIPTION: Atomic Model Cash
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

/** include files **/
#include "cash.h"      // class Cash
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include <iostream>

unsigned short Cash::currentCashToCreate=0;
unsigned short Cash::customerId=0;


/** public functions **/

/*******************************************************************
* Function Name: Cash
* Description: 
********************************************************************/
Cash::Cash( const string &name)
: Atomic(name)
, searchTimePort(addInputPort(SEARCH_TIME_DISTRIBUTION))
, travelTimePort(addInputPort(TRAVEL_TIME_DISTRIBUTION))
, numberOfItemsPort( addInputPort( NUMBER_OF_ITEMS_DISTRIBUTION))
, carOrNotPort(addInputPort( CAR_OR_NOT))
, whichCash(addInputPort(WHICH_CASH))
, bagId(addOutputPort(BAG_ID))
, bagSearchTime(addOutputPort(BAG_SEARCH_TIME))
, customerOutput(addOutputPort(CUSTOMER_OUTPUT))
, oneLessCustomerPort(addOutputPort(ONE_LESS_CUSTOMER))
, customerFinished(addOutputPort(CUSTOMER_FINISHED))
, travelTimeOutput(addOutputPort(OUTPUT_TRAVEL_TIME))
{
	customers.erase( customers.begin(), customers.end() ) ;
	noCurrentCustomer(currentCustomer);
	amActiveCash=0;
	cashId=getNextCashNumber();
	amBusy=0;
}

unsigned short Cash::getNextCashNumber()
{
	return currentCashToCreate++;
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Cash::initFunction()
{
	return *this ;
}

void Cash::noCurrentCustomer(customerProperties &currentCustomer)
{
	currentCustomer.numberOfItems=-1;
	currentCustomer.searchTime=-1;
	currentCustomer.travelTime=-1;
	currentCustomer.carOrNot=-1;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &Cash::externalFunction( const ExternalMessage &msg )
{
	if(msg.port() == searchTimePort ||
		msg.port() == travelTimePort ||
		msg.port() == numberOfItemsPort ||
		msg.port() == carOrNotPort)
	{
		if(msg.port()==searchTimePort)
			currentCustomer.searchTime=msg.value();
		else if(msg.port()==travelTimePort)
			currentCustomer.travelTime=msg.value();
		else if(msg.port()==numberOfItemsPort)
			currentCustomer.numberOfItems=msg.value();
		else //carOrNotPort
			currentCustomer.carOrNot=msg.value();

		if(currentCustomer.searchTime!=-1 &&
			currentCustomer.travelTime!=-1 &&
			currentCustomer.numberOfItems!=-1 &&
			currentCustomer.carOrNot!=-1 &&
			amActiveCash)
		{
			currentCustomer.customerId=getUniqueId();
			cout << "enqued customer at cash "<< cashId << endl;
			customers.push_back(currentCustomer);
			if(!amBusy)
			{
				//starting to be processed
				amBusy=1;
				holdIn(active,Time(0,0,currentCustomer.numberOfItems*TIME_PER_ITEM,0));
			}
			noCurrentCustomer(currentCustomer);
		}
	}
	else
	{
		if(msg.port()==whichCash)
			if(cashId==msg.value())
				amActiveCash=1;
			else
				amActiveCash=0;
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &Cash::internalFunction( const InternalMessage &msg )
{
	customers.pop_front();

	//if there are more customers in the queue
	//get
	if(!customers.empty())
	{
		holdIn(active,Time(0,0,customers.front().numberOfItems*TIME_PER_ITEM,0));
		amBusy=1;
	}
	else
	{
		amBusy=0;
		passivate();
	}
	
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &Cash::outputFunction( const InternalMessage &msg )
{
	int uniqueId=(int)customers.front().customerId;

	/*deal with customer finished processing*/
	if(customers.front().carOrNot)
	{
		sendOutput(msg.time(),travelTimeOutput,customers.front().travelTime);
		sendOutput(msg.time(),customerOutput,uniqueId);

		sendOutput(msg.time(),bagSearchTime,customers.front().searchTime);
		sendOutput(msg.time(),bagId,uniqueId);
	}
	else
 		sendOutput(msg.time(),customerFinished,uniqueId);

	cout << "one less customer at cash " << cashId << endl;
	sendOutput(msg.time(),oneLessCustomerPort,cashId);

	return *this ;
}

unsigned short Cash::getUniqueId()
{
	return customerId++;
}

