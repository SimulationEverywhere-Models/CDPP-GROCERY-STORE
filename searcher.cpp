/*******************************************************************
*
*  DESCRIPTION: Atomic Model Searcher
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
#include "searcher.h"      // class Searcher
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include <iostream>

/*******************************************************************
* Function Name: Searcher
* Description: 
********************************************************************/
Searcher::Searcher( const string &name)
: Atomic( name )
, bagSearchTime( addInputPort(BAG_SEARCH_TIME) )
, customerId( addInputPort(CUSTOMER_ID) )
, customerFinished( addOutputPort(CUSTOMER_FINISHED) )
, removeBag( addOutputPort(REMOVE_BAG))
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		searchTimes[counter].customerId=-1;
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Searcher::initFunction()
{
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &Searcher::externalFunction( const ExternalMessage &msg )
{
	if(msg.port() == bagSearchTime)
		searchTime=msg.value();
	else if(msg.port()==customerId)
	{
		/*if there is currently no countdown in progress 
 		  start a new countdown*/
		if(!countdownInProgress())
		{
			//msg.value is customerId
			addCountdown(searchTime,msg.value(),msg.time());
			holdIn(active,Time(0,0,searchTime));
		} else {
			addCountdown(searchTime,msg.value(),msg.time());
			Time soonestCompletion=getSoonestCompletion();
			holdIn(active,soonestCompletion-msg.time());
		}
	}

	return *this;
}

Time Searcher::getSoonestCompletion()
{
	Time soonestCompletion;
	int firstRound=1;

	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
	{
		if(searchTimes[counter].customerId!=-1)
		{
			if(firstRound)
			{
				soonestCompletion=searchTimes[counter].countdownFinish;
				firstRound=0;
			}
			else
				if(searchTimes[counter].countdownFinish<soonestCompletion)
					soonestCompletion=searchTimes[counter].countdownFinish;
		}
	}
	return soonestCompletion;
}

unsigned short Searcher::countdownInProgress()
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(searchTimes[counter].customerId!=-1)
			return 1;
	
	return 0;
}

void Searcher::addCountdown(Value searchTime,Value customerId,Time currentTime)
{
	Time countdownFinish(currentTime+Time(0,0,searchTime));

	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(searchTimes[counter].customerId==-1)
		{
			searchTimes[counter].customerId=(short)customerId;	
			searchTimes[counter].countdownFinish=countdownFinish;	
			break;
		}
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &Searcher::internalFunction( const InternalMessage &msg)
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(searchTimes[counter].customerId!=-1)
			if(searchTimes[counter].countdownFinish == msg.time())
			{
				searchTimes[counter].customerId=-1;
				//check there are more countdowns to do, these will require the scheduling of an
				//internal transition
				if(countdownInProgress())
				{
					Time soonestCompletion=getSoonestCompletion();
					holdIn(active,soonestCompletion-msg.time());
				}
			}

	if(!countdownInProgress())
		passivate();

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &Searcher::outputFunction( const InternalMessage &msg )
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(searchTimes[counter].customerId!=-1)
			if(searchTimes[counter].countdownFinish == msg.time())
			{
				sendOutput(msg.time(),customerFinished,searchTimes[counter].customerId);
				sendOutput(msg.time(),removeBag,searchTimes[counter].customerId);
			}

	return *this ;
}
