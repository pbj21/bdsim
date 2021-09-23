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
#ifndef BDSMUONCOOLER_H
#define BDSMUONCOOLER_H
#include "BDSAcceleratorComponent.hh"

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include <vector>

class BSDFieldInfo;
class G4Material;

namespace BDS
{
  struct MuonCoolerCoilInfo
  {
    G4double innerRadius;
    G4double radialThickness;
    G4double lengthZ;
    G4double currentDensity;
    G4double offsetZ;
    G4Material* material;
  };

  struct MuonCoolerCavityInfo
  {
    G4double offsetZ;
    G4double lengthZ;
    G4double windowThickness;
    G4Material* windowMaterial;
    G4Material* cavityMaterial;
    BDSFieldInfo* fieldRecipe;
  };

  struct MuonCoolerAbsorberInfo
  {
    G4String absorberType;
    G4double cylinderLength;
    G4double cylinderRadius;
    G4double wedgeOpeningAngle;
    G4double wedgeHeight;
    G4double wedgeRotationAngle;
    G4ThreeVector placement;
    G4double wedgeApexToBase;
    G4Material* material;
  };
}

/**
 * @brief A muon cooling module.
 *
 * @author Laurie Nevay
 */

class BDSMuonCooler: public BDSAcceleratorComponent
{
public:
  BDSMuonCooler(const G4String& nameIn,
		G4double                                        lengthIn,
		G4double                                        containerRadiusIn,
		G4Material*                                     surroundingMaterialIn,
		const std::vector<BDS::MuonCoolerCoilInfo>&     coilInfosIn,
		const std::vector<BDS::MuonCoolerCavityInfo>&   cavityInfosIn,
		const std::vector<BDS::MuonCoolerAbsorberInfo>& absorberInfosIn,
		BDSFieldInfo*                                   outerFieldRecipeIn);
  virtual ~BDSMuonCooler();

protected:
  /// Construct geometry.
  virtual void Build();
  
  /// Void function to fulfill BDSAcceleratorComponent requirements.
  virtual void BuildContainerLogicalVolume();

private:
  void BuildCoils();
  void BuildAbsorbers();
  void AttachOuterBField();
  void BuildCavities();
  
  G4double containerRadius;
  G4Material* surroundingMaterial;
  std::vector<BDS::MuonCoolerCoilInfo> coilInfos;
  std::vector<BDS::MuonCoolerCavityInfo> cavityInfos;
  std::vector<BDS::MuonCoolerAbsorberInfo> absorberInfos;
  BDSFieldInfo* outerFieldRecipe;
};

#endif
