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

#ifndef __Generator1_H
#define __Generator1_H

/** include files **/
#include "atomic.h"     // class Atomic
#include "except.h"     // class InvalidMessageException
#include "store.h"

/** forward declarations **/
class Distribution ;

/** declarations **/
class Generator1 : public Atomic
{
public:
	Generator1( const string &name = "Generator1" );				  // Default constructor

	~Generator1();

	virtual string className() const
		{return "Generator1";}

protected:
	Model &initFunction() ;

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	int initial, increment;
	Port &searchTimeDistributionPort,&travelTimeDistributionPort,&numberOfItemsDistributionPort;
	Port &carOrNotPort,&holdOffPort,&okGoPort;

	unsigned short running;
	Distribution *arrivalDistribution;
	Distribution *searchTimeDistribution;
	Distribution *travelTimeDistribution;
	Distribution *numberOfItemsDistribution;

	unsigned short customersSoFar;

};	// class Generator1


#endif   //__Generator1_H 
