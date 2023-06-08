/*******************************************************************
*
*  DESCRIPTION: Atomic Model Traveler
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
#include "traveler.h"      // class Traveler
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include <iostream>


/** public functions **/

/*******************************************************************
* Function Name: Traveler
* Description: 
********************************************************************/
Traveler::Traveler( const string &name)
: Atomic( name )
, travelTimePort( addInputPort(TRAVEL_TIME_DISTRIBUTION) )
, customerIdPort( addInputPort(CUSTOMER_ID) )
, customerIdOutput( addOutputPort( CUSTOMER_OUTPUT) )
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		travelTimes[counter].customerId=-1;
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Traveler::initFunction()
{
	passivate();
	return *this ;
}

Time Traveler::getSoonestCompletion()
{
	Time soonestCompletion;
	int firstRound=1;

	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
	{
		if(travelTimes[counter].customerId!=-1)
		{
			if(firstRound)
			{
				soonestCompletion=travelTimes[counter].countdownFinish;
				firstRound=0;
			}
			else
				if(travelTimes[counter].countdownFinish<soonestCompletion)
					soonestCompletion=travelTimes[counter].countdownFinish;
		}
	}
	return soonestCompletion;
}

unsigned short Traveler::countdownInProgress()
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(travelTimes[counter].customerId!=-1)
			return 1;
	
	return 0;
}

void Traveler::addCountdown(Value travelTime,Value customerId,Time currentTime)
{
	Time countdownFinish(currentTime+Time(0,0,travelTime));

	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(travelTimes[counter].customerId==-1)
		{
			travelTimes[counter].customerId=(short)customerId;	
			travelTimes[counter].countdownFinish=countdownFinish;	
			break;
		}
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &Traveler::internalFunction( const InternalMessage &msg)
{

	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(travelTimes[counter].customerId!=-1)
		{
			if(travelTimes[counter].countdownFinish == msg.time())
			{
				travelTimes[counter].customerId=-1;

				//check there are more countdowns to do, these will require the scheduling of an
				//internal transition
				if(countdownInProgress())
				{
					Time soonestCompletion=getSoonestCompletion();
					holdIn(active,soonestCompletion-msg.time());
				}
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
Model &Traveler::outputFunction( const InternalMessage &msg )
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_TRAVELERS;counter++)
		if(travelTimes[counter].customerId!=-1)
			if(travelTimes[counter].countdownFinish == msg.time())
				sendOutput(msg.time(),customerIdOutput,travelTimes[counter].customerId);

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &Traveler::externalFunction( const ExternalMessage &msg )
{
	if(msg.port() == travelTimePort)
		travelTime=msg.value();
	else if(msg.port()==customerIdPort)
	{
		/*if there is currently no countdown in progress 
 		  start a new countdown*/
		if(!countdownInProgress())
		{
			//msg.value is customerId
			addCountdown(travelTime,msg.value(),msg.time());
			holdIn(active,Time(0,0,travelTime));
		} else {
			addCountdown(travelTime,msg.value(),msg.time());
			Time soonestCompletion=getSoonestCompletion();
			holdIn(active,soonestCompletion-msg.time());
		}
	}
	return *this;
}


