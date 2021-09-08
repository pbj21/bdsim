/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2021.

This file is part of BDSIM.

BDSIM is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation version 3 of the License.

BDSIM is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BDSIM.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "BDSAcceleratorComponent.hh"
#include "BDSMuonCooler.hh"

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include <vector>

BDSMuonCooler::BDSMuonCooler(const G4String& nameIn,
			     G4double        lengthIn,
			     G4double        containerRadiusIn,
			     const std::vector<BDS::MuonCoolerCoilInfo>&   coilInfosIn,
			     const std::vector<BDS::MuonCoolerCavityInfo>& cavityInfosIn,
			     const BDS::MuonCoolerAbsorberInfo&            absorberInfoIn,
			     BDSFieldInfo*   outerFieldRecipeIn):
  BDSAcceleratorComponent(nameIn, lengthIn, 0, "muoncooler", nullptr),
  containerRadius(containerRadiusIn),
  coilInfos(coilInfosIn),
  cavityInfos(cavityInfosIn),
  absorberInfo(absorberInfoIn),
  outerFieldRecipe(outerFieldRecipeIn)
{;}

BDSMuonCooler::~BDSMuonCooler()
{;}

void BDSMuonCooler::Build()
{
  // build container volume

  // build and place coils

  // build and place absorber

  // attach field to outer

  // build and place cavities
  
  // attach fields to cavities
  
  /*
  BDSBeamPipeFactory* factory = BDSBeamPipeFactory::Instance();
  BDSBeamPipe* pipe = factory->CreateBeamPipe(name,
					      chordLength,
					      beamPipeInfo);

  RegisterDaughter(pipe);
  
  // make the beam pipe container, this object's container
  containerLogicalVolume = pipe->GetContainerLogicalVolume();
  containerSolid         = pipe->GetContainerSolid();

  // register vacuum volume (for biasing)
  SetAcceleratorVacuumLogicalVolume(pipe->GetVacuumLogicalVolume());

  // update extents
  InheritExtents(pipe);

  // update faces
  SetInputFaceNormal(pipe->InputFaceNormal());
  SetOutputFaceNormal(pipe->OutputFaceNormal());
  */
}
