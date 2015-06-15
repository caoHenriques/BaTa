
#ifndef BaPrimaryGeneratorAction_hh
#define BaPrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;
	
class BaPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    BaPrimaryGeneratorAction();    
    ~BaPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
  
};

#endif


