#include "BaPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
//#include "G4StoppingPhysics.hh"
//#include "G4HadronElasticPhysics.hh"

BaPhysicsList::BaPhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(2);

  RegisterPhysics(new G4EmStandardPhysics_option4());
  RegisterPhysics(new G4OpticalPhysics());
  //RegisterPhysics(new G4StoppingPhysics());
  //RegisterPhysics(new G4HadronElasticPhysics());

}


BaPhysicsList::~BaPhysicsList()
{ 
}


void BaPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  