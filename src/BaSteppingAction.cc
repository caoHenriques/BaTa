
#include "BaSteppingAction.hh"
//#include "BaDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
//#include "G4LogicalVolume.hh"

#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4VPhysicalVolume.hh"

#include <iostream>
#include <fstream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BaSteppingAction::BaSteppingAction()
: G4UserSteppingAction(),
  _countW1(0),
  _countW2(0),
  _countWT(0),
  _countLB(0),
  _countT(0),
  _boundary(0)
{}
void BaSteppingAction::UserSteppingAction(const G4Step * step){

	
    G4ParticleDefinition* pdef = step->GetTrack()->GetDefinition();
	
	if (!_boundary) {
		  // Get the list of processes defined for the optical photon
		  // and loop through it to find the optical boundary process.
		G4ProcessVector* pv = pdef->GetProcessManager()->GetProcessList();
		for (G4int i=0; i<pv->size(); i++) {

   			if ((*pv)[i]->GetProcessName() == "OpBoundary") {
   			  _boundary = (G4OpBoundaryProcess*) (*pv)[i];
   			  break;
   			}

		}
    }


	
	// Check if the photon has reached a geometry boundary
    if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
		// Check whether the photon has been detected in the boundary

		if (_boundary->GetStatus() == Detection) {
			

			const G4String touchableName =
				 step->GetPostStepPoint()->GetTouchable()->GetVolume()->GetName();
			
			if(touchableName=="laser_detector"){

				//fscanf(file, "%d		%d	%d	%d %d",\
					&_countT, &_countLB, &_countWT, &_countW1, &_countW2);
				
				std::ifstream myfile;
				myfile.open ("output.txt");
				myfile >> _countT >> _countLB >> _countWT >> _countW1 >> _countW2;
				myfile.close();

				_countLB++;

				FILE* file=fopen("output.txt", "w");
				fprintf(file, "%d	%d	%d	%d %d", \
					_countT, _countLB, _countWT, _countW1, _countW2);
				fclose(file);

				G4cout << "laser_detector_log= "<< _countLB << G4endl;
			}
			else if(touchableName=="photosensor1"){

				std::ifstream myfile;
				myfile.open ("output.txt");
				myfile >> _countT >> _countLB >> _countWT >> _countW1 >> _countW2;
				myfile.close();

				_countWT++;
				_countW1++;

				FILE* file=fopen("output.txt", "w");
				fprintf(file, "%d	%d	%d	%d %d", \
					_countT, _countLB, _countWT, _countW1, _countW2);
				fclose(file);

				G4cout << "photosensor1_log= "<< _countW1 << G4endl;
			}
			else if(touchableName=="photosensor2"){

				std::ifstream myfile;
				myfile.open ("output.txt");
				myfile >> _countT >> _countLB >> _countWT >> _countW1 >> _countW2;
				myfile.close();

				_countWT++;
				_countW2++;

				FILE* file=fopen("output.txt", "w");
				fprintf(file, "%d	%d	%d	%d %d", \
					_countT, _countLB, _countWT, _countW1, _countW2);
				fclose(file);

				G4cout << "photosensor2_log= "<< _countW2 << G4endl;
			}

		}
		
	}

	
}

BaSteppingAction::~BaSteppingAction()
{}
