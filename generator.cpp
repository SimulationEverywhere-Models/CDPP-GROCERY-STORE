/*******************************************************************
*
*  DESCRIPTION: class Generator
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
#include "generator1.h"       // base header
#include "message.h"       // class InternalMessage 
#include "mainsimu.h"      // class Simulator
#include "strutil.h"       // str2Value( ... )

/*******************************************************************
* Function Name: Generator1
* Description: constructor
********************************************************************/
Generator1::Generator1( const string &name )
: Atomic( name )
, searchTimeDistributionPort( addOutputPort( SEARCH_TIME_DISTRIBUTION) )
, travelTimeDistributionPort( addOutputPort( TRAVEL_TIME_DISTRIBUTION) )
, numberOfItemsDistributionPort( addOutputPort( NUMBER_OF_ITEMS_DISTRIBUTION) )
, carOrNotPort( addOutputPort( CAR_OR_NOT) )
, holdOffPort( addInputPort( HOLD_OFF) )
, okGoPort( addInputPort( OK_GO) )
, running(1)
{
	try
	{
		arrivalDistribution = Distribution::create( MainSimulator::Instance().getParameter( description(),
									ARRIVAL_DISTRIBUTION)); 


		searchTimeDistribution = Distribution::create( MainSimulator::Instance().getParameter( description(),
									SEARCH_TIME_DISTRIBUTION)); 

		travelTimeDistribution = Distribution::create( MainSimulator::Instance().getParameter( description(),
									TRAVEL_TIME_DISTRIBUTION)); 

		numberOfItemsDistribution = Distribution::create( MainSimulator::Instance().getParameter( description(),
									NUMBER_OF_ITEMS_DISTRIBUTION)); 

		MASSERT(arrivalDistribution);
		MASSERT(searchTimeDistribution);
		MASSERT(travelTimeDistribution);
		MASSERT(numberOfItemsDistribution);

		string parameter( MainSimulator::Instance().getParameter( description(),ARRIVAL_AVERAGE));
		arrivalDistribution->setVar(0,str2Value(parameter)) ;

		parameter=MainSimulator::Instance().getParameter( description(),SEARCH_TIME_AVERAGE);
		searchTimeDistribution->setVar(0,str2Value(parameter));

		parameter=MainSimulator::Instance().getParameter(description(),TRAVEL_TIME_AVERAGE);
		travelTimeDistribution->setVar(0,str2Value(parameter));
		parameter=MainSimulator::Instance().getParameter(description(),TRAVEL_TIME_STANDARD_ERROR);
		travelTimeDistribution->setVar(1,str2Value(parameter));

		parameter=MainSimulator::Instance().getParameter(description(),NUMBER_OF_ITEMS_AVERAGE);
		numberOfItemsDistribution->setVar(0,str2Value(parameter)) ;
		parameter=MainSimulator::Instance().getParameter(description(),NUMBER_OF_ITEMS_STANDARD_ERROR);
		numberOfItemsDistribution->setVar(1,str2Value(parameter)) ;
	} 
	catch( InvalidDistribution &e )
	{
		e.addText( "The model " + description() + " has distribution problems!" ) ;
		e.print(cerr);
		MTHROW( e ) ;
	} 
	catch( MException &e )
	{
		MTHROW( e ) ;
	}
}

/*******************************************************************
* Function Name: initFunction
********************************************************************/
Model &Generator1::initFunction()
{
	holdIn( active, Time::Zero ) ;
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Generator1::internalFunction( const InternalMessage & msg)
{
	if(running)
	{
		if(customersSoFar==NUMBER_OF_CUSTOMERS)
			passivate();
		else
		{
			holdIn( active,Time(static_cast<float>(fabs(arrivalDistribution->get()))));

			++customersSoFar;
			printf("number of customers so far is %d\n",customersSoFar);
		}
	}
	else
		passivate();

	return *this ;
}


/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Generator1::outputFunction( const InternalMessage &msg )
{
	if(running)
	{		
		sendOutput( msg.time(), numberOfItemsDistributionPort,(int) fabs(numberOfItemsDistribution->get())  ) ;
		sendOutput( msg.time(), searchTimeDistributionPort, fabs( searchTimeDistribution->get() ) ) ;
		sendOutput( msg.time(), travelTimeDistributionPort, fabs( travelTimeDistribution->get() ) ) ;
		sendOutput( msg.time(), carOrNotPort, fabs( randomSign(0.5) > 0 ? 1 : 0 ) ) ;
	}

	return *this ;
}

Model &Generator1::externalFunction( const ExternalMessage &msg )
{
	if(msg.port()==holdOffPort)
		running=0;
	else
	{
		if(customersSoFar<NUMBER_OF_CUSTOMERS)
		{
			running=1;

			holdIn( active,Time(static_cast<float>(fabs(arrivalDistribution->get()))));
		}
	}

	return *this;
}

Generator1::~Generator1()
{
	delete numberOfItemsDistribution;
	delete arrivalDistribution;
	delete searchTimeDistribution;
	delete travelTimeDistribution;
}
