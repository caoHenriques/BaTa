
#ifndef BaActionInitialization_h
#define BaActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class BaActionInitialization : public G4VUserActionInitialization
{
  public:
    BaActionInitialization();
    virtual ~BaActionInitialization();

    virtual void Build() const;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
