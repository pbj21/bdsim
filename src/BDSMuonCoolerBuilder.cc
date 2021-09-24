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
#include "BDSDebug.hh"
#include "BDSException.hh"
#include "BDSFieldInfo.hh"
#include "BDSMaterials.hh"
#include "BDSMuonCooler.hh"
#include "BDSMuonCoolerBuilder.hh"
#include "BDSParser.hh"

#include "parser/coolingchannel.h"
#include "parser/element.h"

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <vector>


BDSMuonCooler* BDS::BuildMuonCooler(const GMAD::Element* element)
{
  // retrieve definition from parser
  const GMAD::CoolingChannel* definition = BDSParser::Instance()->GetCoolingChannel(element->coolingDefinition);
  if (!definition)
    {throw BDSException(__METHOD_NAME__, "no such cooling definition \"" + element->coolingDefinition + "\" defined in input.");}
  
  G4double elementChordLength = element->l * CLHEP::m;
  G4double elementRadius = element->horizontalWidth * 0.5 * CLHEP::m;
  
  std::vector<BDS::MuonCoolerCoilInfo> coilInfos = BDS::BuildMuonCoolerCoilInfos(definition);
  // boundary squares in 2D - keep them to reuse for various checks
  std::vector<BDS::SquareCheck> coilSquares = BDS::MuonCoolerSquaresFromCoils(coilInfos);
  BDS::CheckMuonCoolerCoilInfosForOverlaps(definition->name,
                                           element->name,
                                           coilSquares,
                                           elementChordLength,
                                           elementRadius);
  
  std::vector<BDS::MuonCoolerCavityInfo> cavityInfos = BDS::BuildMuonCoolerCavityInfos(definition);
  // boundary squares in 2D - keep them to reuse for various checks
  std::vector<BDS::SquareCheck> cavitySquares = BDS::MuonCoolerSquaresFromCavities(cavityInfos);
  BDS::CheckMuonCoolerCavityInfosForOverlaps(definition->name,
                                             element->name,
                                             cavitySquares,
                                             coilSquares,
                                             elementChordLength,
                                             elementRadius);
  
  std::vector<BDS::MuonCoolerAbsorberInfo> absorberInfos = BDS::BuildMuonCoolerAbsorberInfo(definition);
  BDS::CheckMuonCoolerAbsorberInfoForOverlaps(definition->name,
                                              element->name,
                                              absorberInfos,
                                              cavitySquares,
                                              coilSquares,
                                              elementChordLength,
                                              elementRadius);

  BDSFieldInfo* outerFieldRecipe = nullptr;

  G4Material* surroundingMaterial = BDSMaterials::Instance()->GetMaterial(definition->surroundingMaterial);

  auto result = new BDSMuonCooler(G4String(element->name),
                                  elementChordLength,
				  elementRadius,
          surroundingMaterial,
				  coilInfos,
				  cavityInfos,
				  absorberInfos,
				  outerFieldRecipe);
  return result;
}

std::vector<BDS::MuonCoolerCoilInfo> BDS::BuildMuonCoolerCoilInfos(const GMAD::CoolingChannel* definition)
{
  std::vector<BDS::MuonCoolerCoilInfo> result;
  
  // Check we have matching coil parameter sizes or tolerate 1 variable for all
  G4int nCoils = definition->nCoils;
  std::vector<std::string> coilParamNames = {"coilInnerRadius",
                                             "coilRadialThickness",
                                             "coilLengthZ",
                                             "coilCurrentDensity",
                                             "coilOffsetZ"};
  std::vector<G4bool> coilVarSingleValued;
  std::vector<const std::list<double>*> coilVars = {&(definition->coilInnerRadius),
                                                    &(definition->coilRadialThickness),
                                                    &(definition->coilLengthZ),
                                                    &(definition->coilCurrentDensity),
                                                    &(definition->coilOffsetZ)};
  // convert to vectors from lists
  std::vector<std::vector<double> > coilVarsV;
  coilVarsV.reserve(coilVars.size());
  for (auto l: coilVars)
    {coilVarsV.emplace_back(std::vector<double>{std::cbegin(*l), std::cend(*l)});}

  // check lengths are either 1 or nCoils
  for (G4int i = 0; i < (G4int) coilVarsV.size(); i++)
    {
      const auto& v = coilVarsV[i];
      if (((G4int) v.size() != nCoils && v.size() != 1) || v.empty())
	{
	  G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
	  msg += "number of " + coilParamNames[i] + " doesn't match nCoils (" + std::to_string(nCoils) + ") or isn't 1";
	  throw BDSException(__METHOD_NAME__, msg);
	}
      coilVarSingleValued.push_back(v.size() == 1);
    }
  
  // build coil infos
  G4double ampsPerCM2 = CLHEP::ampere / CLHEP::cm2;
  for (G4int i = 0; i < nCoils; i++)
    {
      BDS::MuonCoolerCoilInfo info = {coilVarSingleValued[i] ? coilVarsV[0][0] * CLHEP::m : coilVarsV[0][i] * CLHEP::m, // innerRadius
				      coilVarSingleValued[i] ? coilVarsV[1][0] * CLHEP::m : coilVarsV[1][i] * CLHEP::m, // radialThickness
				      coilVarSingleValued[i] ? coilVarsV[2][0] * CLHEP::m : coilVarsV[2][i] * CLHEP::m, // lengthZ
				      coilVarSingleValued[i] ? coilVarsV[3][0] * ampsPerCM2 : coilVarsV[3][i] * ampsPerCM2, // currentDensity
				      coilVarSingleValued[i] ? coilVarsV[4][0] * CLHEP::m : coilVarsV[4][i] * CLHEP::m, // offsetZ
				      nullptr // no material for now
      };
      result.push_back(info);
    }
  
  // make materials
  if (definition->coilMaterial.size() == 1)
    {
      G4String materialName = G4String(definition->coilMaterial.front());
      G4Material* material = BDSMaterials::Instance()->GetMaterial(materialName);
      for (auto& info : result)
	{info.material = material;}
    }
  else
    {
    auto matSize = definition->coilMaterial.size();
      if ( ((G4int)matSize != nCoils && matSize != 1) || definition->coilMaterial.empty())
	{
	  G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
	  msg += "number of coilMaterial doesn't match nCoils (" + std::to_string(nCoils) + ") or isn't 1";
	  throw BDSException(__METHOD_NAME__, msg);
	}
      std::vector<std::string> materialNames = {std::begin(definition->coilMaterial), std::end(definition->coilMaterial)};
      for (G4int i = 0; i < (G4int)materialNames.size(); i++)
	{
	  G4Material* material = BDSMaterials::Instance()->GetMaterial(materialNames[i]);
	  result[i].material = material;
	}
    }
  
  if (definition->mirrorCoils)
    {
      result.reserve(2*result.size());
      std::copy(result.rbegin(), result.rend(), std::back_inserter(result));
      for (G4int j = (G4int)result.size() - 1; j > nCoils - 1; j--)
        {result[j].offsetZ *= -1;}
    }
  
  return result;
}

void BDS::CheckMuonCoolerCoilInfosForOverlaps(const G4String& definitionName,
                                              const G4String& elementName,
                                              const std::vector<BDS::SquareCheck>& coilSquares,
                                              G4double elementChordLength,
                                              G4double elementRadius)
{
  // To check the overlaps, we will do it only in 2D in a cross-section considering the square
  // polygons of the coil blocks. We will do a very simple point in square check - doesn't work
  // if squares are rotated for example.
  
  // check each against each other
  for (G4int i = 0; i < (G4int)coilSquares.size(); i++)
    {
      for (G4int j = i+1; j < (G4int)coilSquares.size(); j++)
	{
	  if (i == j)
	    {continue;}
	  if (coilSquares[i].Overlaps(coilSquares[j]))
	    {
	      G4String msg = "error in definition \"" + definitionName + "\": coil #" + std::to_string(i);
	      msg += " overlaps with coil #" + std::to_string(j) + "\n" + coilSquares[i].Str() + "\n" + coilSquares[j].Str();
	      throw BDSException(__METHOD_NAME__,msg);
	    }
	}
    }
  
  // check all lie inside the container length and radius
  G4double halfLength = 0.5*elementChordLength;
  for (G4int i = 0; i < (G4int)coilSquares.size(); i++)
    {
      if (std::abs(coilSquares[i].z1) > halfLength || std::abs(coilSquares[i].z2) > halfLength)
	{
	  G4String msg = "error in definition \"" + definitionName + "\": coil #" + std::to_string(i);
	  msg += " lies outside +- the half length (" + std::to_string(halfLength) + " mm) of the element \"" + elementName + "\"\n";
	  msg += coilSquares[i].Str();
	  throw BDSException(__METHOD_NAME__,msg);
	}
      if (coilSquares[i].r2 >= elementRadius)
	{
	  G4String msg = "error in definition \"" + definitionName + "\": coil #" + std::to_string(i);
	  msg += " outer radius (" + std::to_string(coilSquares[i].r2) + " mm) is greater than 1/2 the horizontalWidth of the element \"" + elementName + "\"\n";
	  msg += elementName + ": horizontalWidth = " + std::to_string(elementRadius) + " mm\n";
	  msg += "Coil cross-section: " + coilSquares[i].Str();
	  throw BDSException(__METHOD_NAME__,msg);
	}
    }
}

std::vector<BDS::MuonCoolerAbsorberInfo> BDS::BuildMuonCoolerAbsorberInfo(const GMAD::CoolingChannel* definition)
{
  std::vector<BDS::MuonCoolerAbsorberInfo> result;
  
  // Check we have matching coil parameter sizes or tolerate 1 variable for all
  G4int nAbsorbers = definition->nAbsorbers;
  std::vector<std::string> absParamNames = {"absorberOffsetZ",
                                            "absorberCylinderLength",
                                            "absorberCylinderRadius",
                                            "absorberWedgeOpeningAngle",
                                            "absorberWedgeHeight",
                                            "absorberWedgeRotationAngle",
                                            "absorberWedgeOffsetX",
                                            "absorberWedgeOffsetY",
                                            "absorberWedgeApexToBase"};
  std::vector<G4bool> absVarSingleValued;
  std::vector<const std::list<double>*> absVars = {&(definition->absorberOffsetZ),
                                                   &(definition->absorberCylinderLength),
                                                   &(definition->absorberCylinderRadius),
                                                   &(definition->absorberWedgeOpeningAngle),
                                                   &(definition->absorberWedgeHeight),
                                                   &(definition->absorberWedgeRotationAngle),
                                                   &(definition->absorberWedgeOffsetX),
                                                   &(definition->absorberWedgeOffsetY),
                                                   &(definition->absorberWedgeApexToBase)};
  // convert to vectors from lists
  std::vector<std::vector<double> > absVarsV;
  absVarsV.reserve(absVars.size());
  for (auto l: absVars)
    {absVarsV.emplace_back(std::vector<double>{std::cbegin(*l), std::cend(*l)});}
  
  // check lengths are either 1 or nAbsorbers
  for (G4int i = 0; i < (G4int) absVarsV.size(); i++)
    {
      const auto& v = absVarsV[i];
      if (((G4int) v.size() != nAbsorbers && v.size() != 1) || v.empty())
	{
	  G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
	  msg += "number of " + absParamNames[i] + " doesn't match nAbsorbers (" + std::to_string(nAbsorbers) + ") or isn't 1";
	  throw BDSException(__METHOD_NAME__, msg);
	}
      absVarSingleValued.push_back(v.size() == 1);
    }
  
  // check the absorber type list
  const auto typeListSize = definition->absorberType.size();
  if (definition->absorberType.empty() || (typeListSize != 1 && (G4int)typeListSize != nAbsorbers))
    {
      G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
      msg += "number of \"absorberType\" doesn't match nAbsorbers (" + std::to_string(nAbsorbers) + ") or isn't 1";
      throw BDSException(__METHOD_NAME__, msg);
    }
  G4bool absorberTypeSingleValued = typeListSize == 1;
  
  // check contents of absorber type list
  const std::set<std::string> absorberTypes = {"wedge", "cylinder"};
  std::vector<std::string> absorberTypeV = {definition->absorberType.begin(), definition->absorberType.end()};
  for (G4int i = 0; i < (G4int)absorberTypeV.size(); i++)
    {
      auto search = absorberTypes.find(absorberTypeV[i]);
      if (search == absorberTypes.end())
	{
	  G4String msg = "unknown type of absorber: \"" + absorberTypeV[i] + "\" at index " + std::to_string(i);
	  msg += "\nin definition \"" + definition->name + "\"";
	  throw BDSException(__METHOD_NAME__, msg);
	}
    }
  
  // check / prepare absorber material
  const auto materialListSize = definition->absorberMaterial.size();
  if (definition->absorberMaterial.empty() || (materialListSize != 1 && (G4int)materialListSize != nAbsorbers))
    {
      G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
      msg += "number of \"absorberMaterial\" doesn't match nAbsorbers (" + std::to_string(nAbsorbers) + ") or isn't 1";
      throw BDSException(__METHOD_NAME__, msg);
    }
  std::vector<std::string> absorberMaterialNameV = {definition->absorberMaterial.begin(), definition->absorberMaterial.end()};
  G4bool absorberMaterialSingleValued = materialListSize == 1;
  std::map<std::string, G4Material*> absorberMaterials;
  for (const auto& materialName : definition->absorberMaterial) // this may overwrite with duplicates - fine
    {absorberMaterials[materialName] = BDSMaterials::Instance()->GetMaterial(materialName);}
  
  // build absorber infos
  for (G4int i = 0; i < nAbsorbers; i++)
    {
      G4double dx = absVarSingleValued[i] ? absVarsV[6][0] * CLHEP::m : absVarsV[6][i] * CLHEP::m; // wedgeOffsetX
      G4double dy = absVarSingleValued[i] ? absVarsV[7][0] * CLHEP::m : absVarsV[7][i] * CLHEP::m; // wedgeOffsetY
      G4double dz = absVarSingleValued[i] ? absVarsV[0][0] * CLHEP::m : absVarsV[0][i] * CLHEP::m;
      std::string matName = absorberMaterialSingleValued ? absorberMaterialNameV[0] : absorberMaterialNameV[i];
      G4Material* mat = absorberMaterials[matName];
      BDS::MuonCoolerAbsorberInfo info = {absorberTypeSingleValued ? absorberTypeV[0] : absorberTypeV[i],                    // absorberType
					  absVarSingleValued[i] ? absVarsV[1][0] * CLHEP::m   : absVarsV[1][i] * CLHEP::m,   // cylinderLength
					  absVarSingleValued[i] ? absVarsV[2][0] * CLHEP::m   : absVarsV[2][i] * CLHEP::m,   // cylinderRadius
					  absVarSingleValued[i] ? absVarsV[3][0] * CLHEP::rad : absVarsV[3][i] * CLHEP::rad, // wedgeOpeningAngle
					  absVarSingleValued[i] ? absVarsV[4][0] * CLHEP::m   : absVarsV[4][i] * CLHEP::m,   // wedgeHeight
					  absVarSingleValued[i] ? absVarsV[5][0] * CLHEP::rad : absVarsV[5][i] * CLHEP::rad, // wedgeRotationAngle
					  G4ThreeVector(dx,dy,dz),
					  absVarSingleValued[i] ? absVarsV[8][0] * CLHEP::m   : absVarsV[8][i] * CLHEP::m,   // wedgeApexToBase
					  mat
      };
      result.push_back(info);
    }
  
  return result;
}

void BDS::CheckMuonCoolerAbsorberInfoForOverlaps(const G4String& definitionName,
                                                 const G4String& elementName,
                                                 const std::vector<BDS::MuonCoolerAbsorberInfo>& absorberInfos,
                                                 const std::vector<BDS::SquareCheck>& coilInfos,
                                                 const std::vector<BDS::SquareCheck>& cavityInfos,
                                                 G4double elementChordLength,
                                                 G4double elementRadius)
{;}

std::vector<BDS::MuonCoolerCavityInfo> BDS::BuildMuonCoolerCavityInfos(const GMAD::CoolingChannel* definition)
{
  std::vector<BDS::MuonCoolerCavityInfo> result;
  
  /*
  // Check we have matching coil parameter sizes or tolerate 1 variable for all
  G4int nRFCavities = definition->nRFCavities;
  std::vector<std::string> rfParamNames = {"coilInnerRadius",
                                             "coilRadialThickness",
                                             "coilLengthZ",
                                             "coilCurrentDensity",
                                             "coilOffsetZ"};
  std::vector<G4bool> rfVarSingleValued;
  std::vector<const std::list<double>*> rfVars = {&(definition->coilInnerRadius),
                                                    &(definition->coilRadialThickness),
                                                    &(definition->coilLengthZ),
                                                    &(definition->coilCurrentDensity),
                                                    &(definition->coilOffsetZ)};
  // convert to vectors from lists
  std::vector<std::vector<double> > coilVarsV;
  coilVarsV.reserve(coilVars.size());
  for (auto l: coilVars)
  {coilVarsV.emplace_back(std::vector<double>{std::cbegin(*l), std::cend(*l)});}
  
  // check lengths are either 1 or nCoils
  for (G4int i = 0; i < (G4int) coilVarsV.size(); i++)
  {
    const auto& v = coilVarsV[i];
    if (((G4int) v.size() != nRFCavities && v.size() != 1) || v.empty())
    {
      G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
      msg += "number of " + coilParamNames[i] + " doesn't match nCoils (" + std::to_string(nRFCavities) + ") or isn't 1";
      throw BDSException(__METHOD_NAME__, msg);
    }
    coilVarSingleValued.push_back(v.size() == 1);
  }
  
  // build coil infos
  G4double ampsPerCM2 = CLHEP::ampere / CLHEP::cm2;
  for (G4int i = 0; i < nRFCavities; i++)
  {
    BDS::MuonCoolerCoilInfo info = {coilVarSingleValued[i] ? coilVarsV[0][0] * CLHEP::m : coilVarsV[0][i] * CLHEP::m, // innerRadius
                                    coilVarSingleValued[i] ? coilVarsV[1][0] * CLHEP::m : coilVarsV[1][i] * CLHEP::m, // radialThickness
                                    coilVarSingleValued[i] ? coilVarsV[2][0] * CLHEP::m : coilVarsV[2][i] * CLHEP::m, // lengthZ
                                    coilVarSingleValued[i] ? coilVarsV[3][0] * ampsPerCM2 : coilVarsV[3][i] * ampsPerCM2, // currentDensity
                                    coilVarSingleValued[i] ? coilVarsV[4][0] * CLHEP::m : coilVarsV[4][i] * CLHEP::m, // offsetZ
                                    nullptr // no material for now
    };
    result.push_back(info);
  }
  
  // make materials
  if (definition->coilMaterial.size() == 1)
  {
    G4String materialName = G4String(definition->coilMaterial.front());
    G4Material* material = BDSMaterials::Instance()->GetMaterial(materialName);
    for (auto& info : result)
    {info.material = material;}
  }
  else
  {
    auto matSize = definition->coilMaterial.size();
    if ( ((G4int)matSize != nRFCavities && matSize != 1) || definition->coilMaterial.empty())
    {
      G4String msg = "error in coolingchannel definition \"" + definition->name + "\"\n";
      msg += "number of coilMaterial doesn't match nCoils (" + std::to_string(nRFCavities) + ") or isn't 1";
      throw BDSException(__METHOD_NAME__, msg);
    }
    std::vector<std::string> materialNames = {std::begin(definition->coilMaterial), std::end(definition->coilMaterial)};
    for (G4int i = 0; i < (G4int)materialNames.size(); i++)
    {
      G4Material* material = BDSMaterials::Instance()->GetMaterial(materialNames[i]);
      result[i].material = material;
    }
  }
   */
  return result;
}

void BDS::CheckMuonCoolerCavityInfosForOverlaps(const G4String& definitionName,
                                                const G4String& elementName,
                                                const std::vector<BDS::SquareCheck>& cavitySquares,
                                                const std::vector<BDS::SquareCheck>& coilSquares,
                                                G4double elementChordLength,
                                                G4double elementRadius)
{;}

std::vector<BDS::SquareCheck> BDS::MuonCoolerSquaresFromCoils(const std::vector<BDS::MuonCoolerCoilInfo>& coilInfos)
{
  std::vector<BDS::SquareCheck> squares;
  squares.reserve(coilInfos.size());
  for (auto& info : coilInfos)
  {
    squares.emplace_back(BDS::SquareCheck{info.offsetZ-0.5*info.lengthZ,
                                          info.offsetZ+0.5*info.lengthZ,
                                          info.innerRadius,
                                          info.innerRadius+info.radialThickness});
  }
  return squares;
}

std::vector<BDS::SquareCheck> BDS::MuonCoolerSquaresFromCavities(const std::vector<BDS::MuonCoolerCavityInfo>& cavityInfos)
{
  std::vector<BDS::SquareCheck> squares;
  squares.reserve(cavityInfos.size());
  /*
  for (auto& info : cavityInfos)
  {
    squares.emplace_back(BDS::SquareCheck{info.offsetZ-0.5*info.lengthZ,
                                          info.offsetZ+0.5*info.lengthZ,
                                          info.innerRadius,
                                          info.innerRadius+info.radialThickness});
  }
   */
  return squares;
}
