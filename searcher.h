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

#ifndef __SEARCHER_H
#define __SEARCHER_H

#include <list>
#include "atomic.h"     // class Atomic
#include "store.h"

class Searcher : public Atomic
{
public:
	Searcher( const string &name = "Searcher");	//Default constructor

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

	Port &bagSearchTime,&customerId,&customerFinished,&removeBag;

	Value searchTime;
	Countdown searchTimes[MAXIMUM_NUMBER_OF_SEARCHERS];

	list<customerProperties> customers;
};	// class Searcher

// ** inline ** // 
inline
string Searcher::className() const
{
	return "Searcher" ;
}

#endif   //__CASH_H
