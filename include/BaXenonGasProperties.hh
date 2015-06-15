// ----------------------------------------------------------------------------
///  \file   BaXenonGasProperties.h
///  \brief  
///  
///  \author  <justo.martin-albo@ific.uv.es>
///  \date    25 Dec 2010
///  \version $Id$
///
///  Copyright (c) 2010-2012 NEXT Collaboration. All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __XENON_GAS_PROPERTIES__
#define __XENON_GAS_PROPERTIES__

#include <globals.hh>
#include <vector>

class G4MaterialPropertiesTable;


//namespace nexus {

  class BaXenonGasProperties
  {
  public:
    /// Constructor
    BaXenonGasProperties(G4double pressure, G4double temperature);
    /// Destructor
    ~BaXenonGasProperties();

	// Return desity
	G4double Density();

    /// Return the refractive index of xenon gas for a given photon energy
    G4double RefractiveIndex(G4double energy);

	// Rayleigh sacattering
	G4double Rayleigh(G4double energy, G4double rindex);
    

  private:
    G4double _pressure, _temperature;
    G4double _density;
	G4double N_density;


    //static const G4double _densities[100];
  
  };

//} // end namespace nexus

#endif