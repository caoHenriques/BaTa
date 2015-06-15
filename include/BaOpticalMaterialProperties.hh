// ----------------------------------------------------------------------------
///  \file   BaOpticalMaterialProperties.h
///  \brief  Optical properties of most used materials.
///  
///  \author  <justo.martin-albo@ific.uv.es>
///           <francesc.monrabal4@ific.uv.es>
///           <paola.ferrario@ific.uv.es>
///  \date    27 Mar 2009
///  \version $Id$
///
///  Copyright (c) 2009, 2010, 2011 NEXT Collaboration
// ----------------------------------------------------------------------------

#include <globals.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#ifndef __OPTICAL_MATERIAL_PROPERTIES__
#define __OPTICAL_MATERIAL_PROPERTIES__

class G4MaterialPropertiesTable;

//namespace nexus {

  using namespace CLHEP;

  /// Optical properties of several materials. 
  /// (N.B. This is a stateless class where all methods are static functions.)



class BaOpticalMaterialProperties
{
public:

static G4MaterialPropertiesTable* GXe(G4double pressure=1.*bar, 
					G4double temperature=STP_Temperature);

private:
/// Constructor (hidden)
BaOpticalMaterialProperties();
/// Destructor (hidden)
~BaOpticalMaterialProperties();
};

//} // end namespace nexus

#endif