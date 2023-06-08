/*******************************************************************
*
*  DESCRIPTION: Atomic Model Conveyor
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
#include "conveyor.h"      // class Conveyor
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include <iostream>

/*******************************************************************
* Function Name: Conveyor
* Description: 
********************************************************************/
Conveyor::Conveyor( const string &name)
: Atomic(name)
, removeBag(addInputPort(REMOVE_BAG))
, bagId(addInputPort(BAG_ID))
, bagRemoved(addOutputPort(BAG_REMOVED))
{
	for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_BAGS;counter++)
		bags[counter]=-1;
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Conveyor::initFunction()
{
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &Conveyor::externalFunction( const ExternalMessage &msg )
{
	if(msg.port() == bagId)
	{
		for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_BAGS;counter++)
			if(bags[counter]==-1)
			{
				bags[counter]=(short int)msg.value();
				break;
			}
	}
	else//port is removeBag
		for(register unsigned short counter=0;counter<MAXIMUM_NUMBER_OF_BAGS;counter++)
			if(bags[counter]==msg.value())
			{
				bagRemovedId=bags[counter];
				bags[counter]=-1;
				holdIn(active,Time::Zero);
			}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &Conveyor::internalFunction( const InternalMessage &msg )
{
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &Conveyor::outputFunction( const InternalMessage &msg )
{
	sendOutput(msg.time(),bagRemoved,bagRemovedId);
	return *this;
}
