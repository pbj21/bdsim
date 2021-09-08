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
#include "BDSFieldInfo.hh"
#include "BDSMuonCooler.hh"
#include "BDSMuonCoolerBuilder.hh"

#include "parser/element.h"

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include <string>
#include <vector>


BDSMuonCooler* BDS::BuildMuonCooler(const GMAD::Element* element)
{

  std::vector<BDS::MuonCoolerCoilInfo> coilInfos;
  std::vector<BDS::MuonCoolerCavityInfo> cavityInfos;
  BDS::MuonCoolerAbsorberInfo absorberInfo;

  BDSFieldInfo* outerFieldRecipe = nullptr;

  auto result = new BDSMuonCooler(G4String(element->name),
				  element->l * CLHEP::m,
				  element->horizontalWidth * CLHEP::m * 0.5,
				  coilInfos,
				  cavityInfos,
				  absorberInfo,
				  outerFieldRecipe);
  return result;
}
