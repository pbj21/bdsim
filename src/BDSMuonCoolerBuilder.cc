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
  BDS::CheckMuonCoolerCoilInfosForOverlaps(definition->name, element->name, coilInfos, elementChordLength, elementRadius);
  
  std::vector<BDS::MuonCoolerCavityInfo> cavityInfos = BDS::BuildMuonCoolerCavityInfos(definition);
  BDS::CheckMuonCoolerCavityInfosForOverlaps(definition->name, element->name, cavityInfos, elementChordLength);
  
  BDS::MuonCoolerAbsorberInfo absorberInfo = BDS::BuildMuonCoolerAbsorberInfo(definition);
  BDS::CheckMuonCoolerAbsorberInfoForOverlaps(definition->name, element->name, absorberInfo, elementChordLength);

  BDSFieldInfo* outerFieldRecipe = nullptr;

  auto result = new BDSMuonCooler(G4String(element->name),
                                  elementChordLength,
				  elementRadius,
				  coilInfos,
				  cavityInfos,
				  absorberInfo,
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
                                              const std::vector<BDS::MuonCoolerCoilInfo>& coilInfos,
                                              G4double elementChordLength,
                                              G4double elementRadius)
{
  // To check the overlaps, we will do it only in 2D in a cross-section considering the square
  // polygons of the coil blocks. We will do a very simple point in square check - doesn't work
  // if squares are rotated for example.
  struct square
  {
    G4double z1;
    G4double z2;
    G4double r1;
    G4double r2;
    
    G4bool Overlaps(const square& o) const
    {
      G4bool zOK = (o.z1 > z2 || o.z1 < z1) && (o.z2 > z2 || o.z2 < z1) && (o.z2 > o.z1);
      G4bool rOK = (o.r1 > r2 || o.r1 < r1) && (o.r2 > r2 || o.r2 < r1) && (o.r2 > o.r1);
      return !zOK && !rOK;
    }
    G4String Str() const {return "z1: " + std::to_string(z1) + ", z2: " + std::to_string(z2) + ", r1: " + std::to_string(r1) + ", r2: " + std::to_string(r2) + " (mm)";}
  };
  std::vector<square> squares;
  squares.reserve(coilInfos.size());
  for (auto& info : coilInfos)
    {
      squares.emplace_back(square{info.offsetZ-0.5*info.lengthZ,
				  info.offsetZ+0.5*info.lengthZ,
				  info.innerRadius,
				  info.innerRadius+info.radialThickness});
    }
  
  // check each against each other
  for (G4int i = 0; i < (G4int)squares.size(); i++)
    {
      for (G4int j = i+1; j < (G4int)squares.size(); j++)
	{
	  if (i == j)
	    {continue;}
	  if (squares[i].Overlaps(squares[j]))
	    {
	      G4String msg = "error in definition \"" + definitionName + "\": coil #" + std::to_string(i);
	      msg += " overlaps with coil #" + std::to_string(j) + "\n" + squares[i].Str() + "\n" + squares[j].Str();
	      throw BDSException(__METHOD_NAME__,msg);
	    }
	}
    }
  
  // check all lie inside the container length and radius
  G4double halfLength = 0.5*elementChordLength;
  for (G4int i = 0; i < (G4int)squares.size(); i++)
    {
      if (std::abs(squares[i].z1) > halfLength || std::abs(squares[i].z2) > halfLength)
	{
	  G4String msg = "error in definition \"" + definitionName + "\": coil #" + std::to_string(i);
	  msg += " lies outside +- the half length (" + std::to_string(halfLength) + " mm) of the element \"" + elementName + "\"\n";
	  msg += squares[i].Str();
	  throw BDSException(__METHOD_NAME__,msg);
	}
      if (squares[i].r2 >= elementRadius)
	{
	  G4String msg = "error in definition \"" + definitionName + "\": coil #" + std::to_string(i);
	  msg += " outer radius (" + std::to_string(squares[i].r2) + " mm) is greater than 1/2 the horizontalWidth of the element \"" + elementName + "\"\n";
	  msg += elementName + ": horizontalWidth = " + std::to_string(elementRadius) + " mm\n";
	  msg += "Coil cross-section: " + squares[i].Str();
	  throw BDSException(__METHOD_NAME__,msg);
	}
    }
}

std::vector<BDS::MuonCoolerCavityInfo> BDS::BuildMuonCoolerCavityInfos(const GMAD::CoolingChannel* definition)
{
  std::vector<BDS::MuonCoolerCavityInfo> result;
  return result;
}

void BDS::CheckMuonCoolerCavityInfosForOverlaps(const G4String& definitionName,
                                                const G4String& elementName,
                                                const std::vector<BDS::MuonCoolerCavityInfo>& cavityInfos,
                                                G4double elementChordLength)
{;}

BDS::MuonCoolerAbsorberInfo BDS::BuildMuonCoolerAbsorberInfo(const GMAD::CoolingChannel* definition)
{
  if (definition->absorberType.empty())
    {throw BDSException(__METHOD_NAME__, "must set \"absorberType\" in cooling definition \"" + definition->name + "\"");}
  std::set<std::string> absorberTypes = {"wedge", "cylinder"};
  auto search = absorberTypes.find(definition->absorberType);
  if (search == absorberTypes.end())
    {throw BDSException(__METHOD_NAME__, "unknown type of absorber: \"" + definition->absorberType + "\"");}
  return BDS::MuonCoolerAbsorberInfo();
}

void BDS::CheckMuonCoolerAbsorberInfoForOverlaps(const G4String& definitionName,
                                                 const G4String& elementName,
                                                 const BDS::MuonCoolerAbsorberInfo&,
                                                 G4double elementChordLength)
{;}
