/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2022.

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
#include "BDSFieldEMMuonCooler.hh"
#include "BDSFieldInfoExtra.hh"
#include "BDSFieldMagSolenoidBlock.hh"
#include "BDSFieldMagSolenoidSheet.hh"
#include "BDSFieldMagSolenoidLoop.hh"
#include "BDSFieldMagVectorSum.hh"
#include "BDSFieldMag.hh"
#include "BDSFieldType.hh"
#include "BDSMuonCoolerStructs.hh"
#include "BDSUtilities.hh"

#include "CLHEP/Units/PhysicalConstants.h"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include <utility>

BDSFieldEMMuonCooler::BDSFieldEMMuonCooler(const BDSFieldInfoExtraMuonCooler* info,
                                           G4double brho):
  coilField(nullptr),
  rfField(nullptr)
{
  switch (info->magneticFieldType.underlying())
    {
    case BDSFieldType::solenoidblock:
      {
        const auto& coilInfos = info->coilInfos;
        std::vector<BDSFieldMag*> fields;
        std::vector<G4ThreeVector> fieldOffsets;
        for (const auto& ci : coilInfos)
	  {
	    fields.push_back(new BDSFieldMagSolenoidBlock(ci.current,
							  true,
							  ci.innerRadius,
							  ci.radialThickness,
							  ci.fullLengthZ));
	    fieldOffsets.emplace_back(G4ThreeVector(0,0,ci.offsetZ));
	  }
        coilField = new BDSFieldMagVectorSum(fields, fieldOffsets);
        break;
      }
    case BDSFieldType::solenoidsheet:
      {
	const auto& coilInfos = info->coilInfos;
	std::vector<BDSFieldMag*> fields;
	std::vector<G4ThreeVector> fieldOffsets;
	for (const auto& ci : coilInfos)
	  {
	    fields.push_back(new BDSFieldMagSolenoidSheet(ci.current,
							  true,
							  ci.innerRadius + 0.5*ci.radialThickness,
							  ci.fullLengthZ));
	    fieldOffsets.emplace_back(G4ThreeVector(0,0,ci.offsetZ));
	  }
	coilField = new BDSFieldMagVectorSum(fields, fieldOffsets);
	break;
      }
    case BDSFieldType::solenoidloop:
      {
	const auto& coilInfos = info->coilInfos;
	std::vector<BDSFieldMag*> fields;
	std::vector<G4ThreeVector> fieldOffsets;
	for (const auto& ci : coilInfos)
	  {
	    fields.push_back(new BDSFieldMagSolenoidLoop(ci.current,
							 true,
							 ci.innerRadius + 0.5*ci.radialThickness));
	    fieldOffsets.emplace_back(G4ThreeVector(0,0,ci.offsetZ));
	  }
	coilField = new BDSFieldMagVectorSum(fields, fieldOffsets);
	break;
      }
    default:
      {
	G4String msg = "\"" + info->magneticFieldType.ToString();
	msg += "\" is not a valid field model for a muon cooler B field";
	throw BDSException(__METHOD_NAME__, msg);
	break;
      }
    }
}

BDSFieldEMMuonCooler::~BDSFieldEMMuonCooler()
{
  delete coilField;
  delete rfField;
}

std::pair<G4ThreeVector, G4ThreeVector> BDSFieldEMMuonCooler::GetField(const G4ThreeVector& position,
								       const G4double       t) const
{
  auto cf  = coilField->GetField(position, t);
  //auto rff = rfField->GetField(position, t);
  auto rff = std::make_pair(G4ThreeVector(), G4ThreeVector());
  // only rf has E field, but both have B -> sum B field
  G4ThreeVector b  = cf + rff.first;
  
  auto result = std::make_pair(b, rff.second);
  return result;
}
