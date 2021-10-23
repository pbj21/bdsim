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
#include "BDSArray1DCoords.hh"
#include "BDSFieldValue.hh"
#include "BDSInterpolator1DLinearMag.hh"
#include "BDSInterpolatorRoutines.hh"

#include "globals.hh"

#include <cmath>

BDSInterpolator1DLinearMag::BDSInterpolator1DLinearMag(BDSArray1DCoords* arrayIn):
  BDSInterpolator1D(arrayIn)
{;}

BDSInterpolator1DLinearMag::~BDSInterpolator1DLinearMag()
{;}

BDSFieldValue BDSInterpolator1DLinearMag::GetInterpolatedValueT(G4double x) const
{
  G4double xarr = array->ArrayCoordsFromX(x);
  G4double x1 = std::floor(xarr);

  BDSFieldValue values[2];
  values[0] = array->GetConst((G4int)x1);
  values[1] = array->GetConst((G4int)x1+1);
  BDSFieldValue result = BDS::Linear1DMag(values, xarr-x1);
  
  return result;
}
