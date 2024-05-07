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
#include "BDSFieldEM.hh"
#include "BDSFieldEMVectorSum.hh"

#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include <utility>
#include <vector>

BDSFieldEMVectorSum::BDSFieldEMVectorSum(const std::vector<BDSFieldEM*>& fieldsIn,
                                         const std::vector<G4ThreeVector>& fieldOffsetsIn,
                                         const std::vector<G4double>& timeOffsetsIn):
  fields(fieldsIn),
  fieldOffsets(fieldOffsetsIn),
  timeOffsets(timeOffsetsIn)
{
  if (fields.size() != fieldOffsets.size() || fields.size() != timeOffsets.size())
    {throw BDSException(__METHOD_NAME__, "number of fields, offsets and time offsets do not match");}
}

BDSFieldEMVectorSum::~BDSFieldEMVectorSum()
{
  for (auto f : fields)
    {delete f;}
}

std::pair<G4ThreeVector,G4ThreeVector> BDSFieldEMVectorSum::GetField(const G4ThreeVector& position,
                                                                     const G4double       t) const
{
  std::pair<G4ThreeVector, G4ThreeVector> result;
  for (G4int i = 0; i < (G4int)fields.size(); i++)
    {
      G4ThreeVector p = position - fieldOffsets[i];
      G4double ti = t - timeOffsets[i];
      std::pair<G4ThreeVector, G4ThreeVector> ri = fields[i]->GetField(p,ti);
      result.first += ri.first;
      result.second += ri.second;
    }
  return result;
}
