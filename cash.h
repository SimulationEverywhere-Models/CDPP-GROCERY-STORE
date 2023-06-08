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

#ifndef __CASH_H
#define __CASH_H

#include <list>
#include "atomic.h"     // class Atomic
#include "store.h"

class Cash : public Atomic
{
public:
	Cash( const string &name = "Cash");	//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	void noCurrentCustomer(customerProperties &currentCustomer);
	static unsigned short getNextCashNumber();
	static unsigned short getUniqueId(void);

	unsigned short cashId;

	Port &searchTimePort,&travelTimePort,&numberOfItemsPort,&carOrNotPort,&whichCash;
	Port &bagId,&bagSearchTime,&customerOutput,&oneLessCustomerPort,&customerFinished,&travelTimeOutput;

	customerProperties currentCustomer;
	short unsigned amActiveCash;
	static unsigned short currentCashToCreate;
	static unsigned short customerId;
	unsigned short amBusy;

	list<customerProperties> customers;
};	// class Cash

// ** inline ** // 
inline
string Cash::className() const
{
	return "Cash" ;
}

#endif   //__CASH_H
