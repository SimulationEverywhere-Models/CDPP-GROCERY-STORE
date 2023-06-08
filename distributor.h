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

#ifndef __DISTRIBUTOR_H
#define __DISTRIBUTOR_H


/** include files **/
#include "atomic.h"     // class Atomic
#include "except.h"     // class InvalidMessageException
#include "store.h"

/** forward declarations **/
class Distribution ;

/** declarations **/
class Distributor : public Atomic
{
public:
	Distributor( const string &name = "Distributor" );				  // Default constructor

	~Distributor();

	virtual string className() const
		{return "Distributor";}

protected:
	Model &initFunction() ;

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	Port &holdOffPort,&okGoPort,&oneLessCustomerPort;
	Port &searchTimeDistributionPort,&travelTimeDistributionPort,&numberOfItemsDistributionPort,&carOrNotPort;
	Port &outputCarOrNot,&outputNumberOfItems,&outputWhichCash,&outputTravelTime,&outputSearchTime;

	unsigned int queueSizes[NUM_QUEUES];
	customerProperties currentCustomer;
	unsigned short outputCash;
	unsigned short sendHoldOff;
	unsigned short okGoPending;

	void noCurrentCustomer(customerProperties&);

	unsigned short findSmallestQueueForExpressCustomer();
	unsigned short findSmallestQueueForNonExpressCustomer();
	

/* There was a method to return dist and there was an internal variable dist*/
};	// class Distributor


#endif   //__DISTRIBUTOR_H
