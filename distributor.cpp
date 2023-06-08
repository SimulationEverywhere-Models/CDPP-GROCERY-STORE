/*******************************************************************
*
*  DESCRIPTION: class Distributor
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
#include "realfunc.h"        // class Distribution 
#include "distri.h"        // class Distribution 
#include "distributor.h"       // base header
#include "message.h"       // class InternalMessage 
#include "mainsimu.h"      // class Simulator
#include "strutil.h"       // str2Value( ... )
#include <iostream>

/*******************************************************************
* Function Name: Distributor
* Description: constructor
********************************************************************/
Distributor::Distributor( const string &name )
: Atomic( name )
, holdOffPort( addOutputPort( HOLD_OFF) ) 
, okGoPort( addOutputPort( OK_GO) )
, oneLessCustomerPort(addInputPort(ONE_LESS_CUSTOMER))
, searchTimeDistributionPort( addInputPort( SEARCH_TIME_DISTRIBUTION) )
, travelTimeDistributionPort( addInputPort( TRAVEL_TIME_DISTRIBUTION) )
, numberOfItemsDistributionPort( addInputPort( NUMBER_OF_ITEMS_DISTRIBUTION) )
, carOrNotPort( addInputPort( CAR_OR_NOT) )
, outputCarOrNot( addOutputPort( OUTPUT_CAR_OR_NOT) )
, outputNumberOfItems( addOutputPort( OUTPUT_NUMBER_OF_ITEMS))
, outputWhichCash( addOutputPort(OUTPUT_WHICH_CASH))
, outputTravelTime( addOutputPort(OUTPUT_TRAVEL_TIME))
, outputSearchTime( addOutputPort(OUTPUT_SEARCH_TIME ))
, sendHoldOff(0)
, okGoPending(0)
{
	noCurrentCustomer(currentCustomer);

	for(register unsigned short counter=0;counter<(unsigned short)NUM_QUEUES;counter++)
		queueSizes[counter]=0;	

	return;
}

void Distributor::noCurrentCustomer(customerProperties &currentCustomer)
{
	currentCustomer.numberOfItems=-1;
	currentCustomer.searchTime=-1;
	currentCustomer.travelTime=-1;
	currentCustomer.carOrNot=-1;
}

/*******************************************************************
* Function Name: initFunction
********************************************************************/
Model &Distributor::initFunction()
{
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Distributor::internalFunction( const InternalMessage & )
{
	if(okGoPending)
	{
		okGoPending=0;
		passivate();
	}
	else
	{
		if(!sendHoldOff)
		{
			noCurrentCustomer(currentCustomer);
			queueSizes[outputCash]++;
		}

		cout << "queue size of " << outputCash << " is " << queueSizes[outputCash] << endl;

		if(queueSizes[0]==(unsigned)MAX_QUEUE_SIZE &&
			queueSizes[1]==(unsigned)MAX_QUEUE_SIZE &&
			queueSizes[2]==(unsigned)MAX_QUEUE_SIZE &&
			queueSizes[3]==(unsigned)MAX_QUEUE_SIZE)
		{

			if(sendHoldOff==0)
			{
				sendHoldOff=1;
				holdIn(active,Time::Zero);
			}
			else
			{
				sendHoldOff=0;
				passivate();
			}
		}
		else
			passivate();
	}

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Distributor::outputFunction( const InternalMessage &msg )
{
	if(okGoPending)
		sendOutput(msg.time(), okGoPort,1) ;
	else
	{
		if(sendHoldOff)
		{
			sendOutput(msg.time(), holdOffPort,1) ;
			cout << endl<<"sent holdOff" << endl;
		}
		else
		{
			sendOutput( msg.time(), outputWhichCash,outputCash) ;
			sendOutput( msg.time(), outputNumberOfItems,currentCustomer.numberOfItems) ;
			sendOutput( msg.time(), outputSearchTime,currentCustomer.searchTime) ;
			sendOutput( msg.time(), outputTravelTime,currentCustomer.travelTime) ;
			sendOutput( msg.time(), outputCarOrNot,currentCustomer.carOrNot) ;
		}
	}

		
	return *this ;
}

/*
BASIC IDEA:
 External transition func: when stuff comes in
		if #items < 8
			if express queue not full
				send to express queue
			else
				determine smallest queue
				if all queues full
					send hold off to distributor
				else
					send to that queue
				end
			end
		end
*/
Model &Distributor::externalFunction( const ExternalMessage &msg )
{
	if(msg.port()==oneLessCustomerPort)
	{
		cout << "oneLessCustomer received at Distributor" << endl;
		queueSizes[(unsigned)msg.value()]--;
		okGoPending=1;
		holdIn( active, Time::Zero ) ;
	}
	else
	{
		if(msg.port() == numberOfItemsDistributionPort)
			currentCustomer.numberOfItems=msg.value();
		if(msg.port() == searchTimeDistributionPort)
			currentCustomer.searchTime=msg.value();
		if(msg.port() == travelTimeDistributionPort)
			currentCustomer.travelTime=msg.value();
		if(msg.port() == carOrNotPort)
			currentCustomer.carOrNot=msg.value();

		if(currentCustomer.numberOfItems!=-1 &&
			currentCustomer.searchTime!=-1 &&
			currentCustomer.travelTime!=-1 &&
			currentCustomer.carOrNot!=-1)
		{
			if(currentCustomer.numberOfItems<EXPRESS_CASH_MAXIMUM) 
				outputCash=findSmallestQueueForExpressCustomer();
			else
				outputCash=findSmallestQueueForNonExpressCustomer();
	
			holdIn( active, Time::Zero ) ;
		}
	}

	
	return *this ;
}

unsigned short Distributor::findSmallestQueueForExpressCustomer()
{
	for(unsigned short counter=0;counter<NUM_QUEUES;counter++)
		if(queueSizes[counter]==0)
			return counter;	
			
	if(queueSizes[EXPRESS_CASH]>=(unsigned)MAX_QUEUE_SIZE)
		return findSmallestQueueForNonExpressCustomer();
	else
		return EXPRESS_CASH;
		
}

unsigned short Distributor::findSmallestQueueForNonExpressCustomer()
{
	unsigned short minimumCash=0;

	for(unsigned short counter=0;counter<NUM_QUEUES;counter++)
		if(queueSizes[counter]<queueSizes[minimumCash])
			minimumCash=counter;	

	return minimumCash;
}

Distributor::~Distributor()
{
}
