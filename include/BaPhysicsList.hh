#ifndef BaPhysicsList_h
#define BaPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class BaPhysicsList: public G4VModularPhysicsList
{
public:
  BaPhysicsList();
  virtual ~BaPhysicsList();

  virtual void SetCuts();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif