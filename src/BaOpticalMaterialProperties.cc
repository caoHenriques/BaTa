#include "BaOpticalMaterialProperties.hh"
#include "BaXenonGasProperties.hh"
#include <G4MaterialPropertiesTable.hh>
#include "G4SystemOfUnits.hh"

//using namespace nexus;
using namespace CLHEP;


G4MaterialPropertiesTable* BaOpticalMaterialProperties::GXe(G4double pressure, 
							  G4double temperature)
{

  BaXenonGasProperties GXe_prop(pressure, temperature);
  G4MaterialPropertiesTable* GXe_mpt = new G4MaterialPropertiesTable();
  
  const G4int n_entries = 100;
  G4double n_energy[n_entries];
  G4double energy_start=   2.4 *eV;
  G4double energy_finish= 2.6 *eV ;

  for(G4int i=0; i<n_entries; i++){
	n_energy[i]=(energy_start+i*(energy_finish-energy_start)/n_entries );
  }

  G4double rayleigh[n_entries];
  G4double rindex[n_entries];
  for (G4int i=0; i<n_entries; i++) {
    rindex[i] = GXe_prop.RefractiveIndex(n_energy[i]);
    //G4cout << n_energy[i] << ", " << rindex[i] << G4endl;
	rayleigh[i] = GXe_prop.Rayleigh(n_energy[i],rindex[i]) *0.00001 *m;
	//G4cout << "Rayleigh= " << n_energy[i] << ", " << 1/(rayleigh[i]/m) << G4endl;
  }

  GXe_mpt->AddProperty("RINDEX", n_energy, rindex, n_entries);
  GXe_mpt->AddProperty("RAYLEIGH", n_energy,rayleigh, n_entries);

//  is it necessary?  
  /*
  GXe_mpt->AddConstProperty("FASTTIMECONSTANT",1.*ns);
  GXe_mpt->AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  GXe_mpt->AddConstProperty("YIELDRATIO",.9);
  GXe_mpt->AddConstProperty("ATTACHMENT", 1000.*ms);
  G4double energy[2] = {0.01*eV, 100.*eV};
  G4double abslen[2] = {1.e8*m, 1.e8*m};
  GXe_mpt->AddProperty("ABSLENGTH", energy, abslen, 2);
  */
 
  return GXe_mpt;
  
}

