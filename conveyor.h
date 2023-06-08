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

#ifndef __CONVEYOR_H
#define __CONVEYOR_H

#include <list>
#include "atomic.h"     // class Atomic
#include "store.h"

class Conveyor : public Atomic
{
public:
	Conveyor( const string &name = "Conveyor");	//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	Port &removeBag,&bagId,&bagRemoved;
	unsigned short bagRemovedId;

	short int bags[MAXIMUM_NUMBER_OF_BAGS];
};	// class Conveyor

// ** inline ** // 
inline
string Conveyor::className() const
{
	return "Conveyor" ;
}

#endif   //__CONVEYOR_H
