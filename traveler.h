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

#ifndef __TRAVELER_H
#define __TRAVELER_H

#include <list>
#include "atomic.h"     // class Atomic
#include "store.h"

class Traveler : public Atomic
{
public:
	Traveler( const string &name = "Traveler");	//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:

	unsigned short countdownInProgress();
	void addCountdown(Value travelTime,Value customerId,Time currentTime);
	Time getSoonestCompletion();

	void noCurrentCustomer(customerProperties &currentCustomer);

	Port &travelTimePort,&customerIdPort,&customerIdOutput;

	Value travelTime;
	Countdown travelTimes[MAXIMUM_NUMBER_OF_TRAVELERS];

	list<customerProperties> customers;
};	// class Traveler

// ** inline ** // 
inline
string Traveler::className() const
{
	return "Traveler" ;
}

#endif   //__CASH_H
