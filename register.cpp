/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "generator1.h"    // class Generator
#include "cash.h"
#include "distributor.h"    // class Generator
#include "store.h"
#include "traveler.h"
#include "searcher.h"
#include "conveyor.h"

void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic(NewAtomicFunction<Generator1>() , "Generator" ) ;
	SingleModelAdm::Instance().registerAtomic(NewAtomicFunction<Distributor>() , "Distributor" ) ;
	SingleModelAdm::Instance().registerAtomic(NewAtomicFunction<Cash>() , "Cash") ;
	SingleModelAdm::Instance().registerAtomic(NewAtomicFunction<Traveler>() , "Traveler") ;
	SingleModelAdm::Instance().registerAtomic(NewAtomicFunction<Searcher>() , "Searcher") ;
	SingleModelAdm::Instance().registerAtomic(NewAtomicFunction<Conveyor>() , "Conveyor") ;
}
