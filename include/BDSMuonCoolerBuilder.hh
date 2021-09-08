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
#ifndef BDSMUONCOOLERBUILDER_H
#define BDSMUONCOOLERBUILDER_H

#include <vector>

class BDSMuonCooler;
namespace GMAD
{
  class CoolingChannel;
  struct Element;
}

namespace BDS
{
  struct MuonCoolerCoilInfo; // forward declaration
  struct MuonCoolerCavityInfo;
  struct MuonCoolerAbsorberInfo;
  
  /// Build a muon cooler beam line element interpreting and checking
  /// all the relevant parameters from the parser definition.
  BDSMuonCooler* BuildMuonCooler(const GMAD::Element* element);
  
  std::vector<BDS::MuonCoolerCoilInfo> BuildMuonCoolerCoilInfos(const GMAD::CoolingChannel* definition);
  
  void CheckMuonCoolerCoilInfosForOverlaps(const G4String& definitionName,
                                           const G4String& elementName,
                                           const std::vector<BDS::MuonCoolerCoilInfo>& coilInfos,
                                           G4double elementChordLength,
                                           G4double elementRadius);
  
  std::vector<BDS::MuonCoolerCavityInfo> BuildMuonCoolerCavityInfos(const GMAD::CoolingChannel* definition);
  
  void CheckMuonCoolerCavityInfosForOverlaps(const G4String& definitionName,
                                             const G4String& elementName,
                                             const std::vector<BDS::MuonCoolerCavityInfo>& cavityInfos,
                                             G4double elementChordLength);
  
  BDS::MuonCoolerAbsorberInfo BuildMuonCoolerAbsorberInfo(const GMAD::CoolingChannel* definition);
  void CheckMuonCoolerAbsorberInfoForOverlaps(const G4String& definitionName,
                                              const G4String& elementName,
                                              const BDS::MuonCoolerAbsorberInfo&,
                                              G4double elementChordLength);
}

#endif
