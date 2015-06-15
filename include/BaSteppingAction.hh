

#ifndef BaSteppingAction_h
#define BaSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4OpBoundaryProcess;
//class B1EventAction;
//class G4LogicalVolume;

/// Stepping action class
/// 

class BaSteppingAction : public G4UserSteppingAction
{
  public:
    BaSteppingAction();
    virtual ~BaSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
	G4int _countLB;
	G4int _countW1;
	G4int _countW2;
	G4int _countT;
	G4int _countWT;
	G4OpBoundaryProcess* _boundary;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
