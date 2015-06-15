
#include "BaActionInitialization.hh"
#include "BaPrimaryGeneratorAction.hh"
#include "BaSteppingAction.hh"

BaActionInitialization::BaActionInitialization()
 : G4VUserActionInitialization()
{}

BaActionInitialization::~BaActionInitialization()
{}


void BaActionInitialization::Build() const
{
	SetUserAction(new BaPrimaryGeneratorAction);
	SetUserAction(new BaSteppingAction);
}  

