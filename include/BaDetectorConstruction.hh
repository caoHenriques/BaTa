

#ifndef BaDetectorConstruction_h
#define BaDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class BaDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    BaDetectorConstruction();
    virtual ~BaDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
	//void ConstructSDandField();
};


#endif

