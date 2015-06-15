// ----------------------------------------------------------------------------
//  $Id$
//
//  Author:  <justo.martin-albo@ific.uv.es>
//  Created: 25 Dec 2010
//
//  Copyright (c) 2010-2012 NEXT Collaboration. All rights reserved.
// ----------------------------------------------------------------------------

#include "BaXenonGasProperties.hh"
#include <G4MaterialPropertiesTable.hh>
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4SystemOfUnits.hh"

//namespace nexus {

  using namespace CLHEP; 
  
  BaXenonGasProperties::BaXenonGasProperties(G4double pressure,
					 G4double temperature):
    _pressure(pressure), _temperature(temperature)
  {
    Density();
  }
  
  
  BaXenonGasProperties::~BaXenonGasProperties()
  {  

  }
  

  
  G4double BaXenonGasProperties::Density()
  {

    // G4double data[50][1] =
    //   {{ 1.0,  5.29074},
    //    { 2.0, 10.63668},
    //    { 3.0, 16.03932},
    //    { 4.0, 21.50023},
    //    { 5.0, 27.02107},
    //   };

    // Ideal gas state equation P*V = n*R*T
    // I'm assuming atmosphere = bar 

    // values for R
    //  8.3145 J mol-1 K-1
    //  0.0831451 L bar K-1 mol-1
    //  82.058 cm3 atm mol-1 K-1
    // value for molar mass: 131.29 g/mol
	
    _density = (_pressure/atmosphere)*131.29/(_temperature/kelvin*82.058)\
		*pow(10.,3);

	N_density =  ((_pressure/atmosphere)\
		* Avogadro*mole / ((82.058)*_temperature/kelvin) *pow(10.,6) );

    return _density, N_density;
	
  }
  

  
  G4double BaXenonGasProperties::RefractiveIndex(G4double energy)
  {
    // Formula for the refractive index taken from
    // A. Baldini et al., "Liquid Xe scintillation calorimetry 
    // and Xe optical properties", arXiv:physics/0401072v1 [physics.ins-det]
    
    // The Lorentz-Lorenz equation (also known as Clausius-Mossotti equation)
    // relates the refractive index of a fluid with its density:
    // (n^2 - 1) / (n^2 + 2) = - A · d_M,     (1)
    // where n is the refractive index, d_M is the molar density and
    // A is the first refractivity viral coefficient:
    // A(E) = \sum_i^3 P_i / (E^2 - E_i^2),   (2)
    // with:
    G4double P[3] = {71.23, 77.75, 1384.89}; // [eV^3 cm3 / mole]
    G4double E[3] = {8.4, 8.81, 13.2};       // [eV]

    // Note.- Equation (1) has, actually, a sign difference with respect 
    // to the one appearing in the reference. Otherwise, it yields values
    // for the refractive index below 1.

    // Let's calculate the virial coefficient.
    // We won't use the implicit system of units of Geant4 because
    // it results in loss of numerical precision.

    energy = energy / eV;
    G4double virial = 0.;

    for (G4int i=0; i<3; i++)
      virial = virial + P[i] / (energy*energy - E[i]*E[i]);
    
    G4double density = 6.E-3;
    G4double mol_density = density / 131.29;
    G4double alpha = virial * mol_density;
    
    // Isolating now the n2 from equation (1) and taking the square root
    G4double n2 = (1. - 2*alpha) / (1. + alpha);

    if (n2 < 1.) {
 //      G4String msg = "Non-physical refractive index for energy "
	// + bhep::to_string(energy) + " eV. Use n=1 instead.";
 //      G4Exception("[BaXenonGasProperties]", "RefractiveIndex()",
	// 	  JustWarning, msg);
      n2 = 1.;
    }
    
    return sqrt(n2);
  }
  
  
  G4double BaXenonGasProperties::Rayleigh(G4double energy, G4double rindex){
	
	G4double waveLength = h_Planck*c_light/(energy);

	// attenuation coefitient= cross section * number of atoms per volume
	G4double att_coef= 8.*pow(pi,3)* \
		pow( pow(rindex,2) -1., 2) \
		/ 3/N_density/pow(waveLength/m,4);

	return 1/att_coef;
  }
  
