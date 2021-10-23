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
#include "BDSArray2DCoords.hh"
#include "BDSFieldValue.hh"
#include "BDSInterpolator2DLinearMag.hh"
#include "BDSInterpolatorRoutines.hh"

#include "globals.hh"

#include <cmath>

BDSInterpolator2DLinearMag::BDSInterpolator2DLinearMag(BDSArray2DCoords* arrayIn):
  BDSInterpolator2D(arrayIn)
{;}

BDSInterpolator2DLinearMag::~BDSInterpolator2DLinearMag()
{;}

BDSFieldValue BDSInterpolator2DLinearMag::GetInterpolatedValueT(G4double x, G4double y) const
{
  G4double xarr = array->ArrayCoordsFromX(x);
  G4double yarr = array->ArrayCoordsFromY(y);
  
  G4double x1 = std::floor(xarr);
  G4double y1 = std::floor(yarr);

  BDSFieldValue localData[2][2];
  for (int i = 0; i < 2; i++)
    {
      for (int j = 0; j < 2; j++)
	{localData[i][j] = array->GetConst((G4int)x1+i, (G4int)y1+j);}
    }
  
  BDSFieldValue result = BDS::Linear2DMag(localData, xarr-x1, yarr-y1);
  
  return result;
}
