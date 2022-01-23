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
#include "BDSFieldE.hh"

#include "globals.hh" // geant4 types / globals
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"

BDSFieldE::BDSFieldE():
  finiteStrength(true),
  transform(G4Transform3D::Identity),
  inverseTransform(G4Transform3D::Identity)
{;}

BDSFieldE::BDSFieldE(G4Transform3D transformIn):
  finiteStrength(true),
  transform(transformIn),
  inverseTransform(transformIn.inverse())
{;}

G4ThreeVector BDSFieldE::GetFieldTransformed(const G4ThreeVector& position,
					     const G4double       t) const
{
  if (!finiteStrength)
    {return G4ThreeVector();} // quicker than query
  else if (transform != G4Transform3D::Identity)
    {
      G4ThreeVector transformedPosition = inverseTransform * (HepGeom::Point3D<G4double>)position;
      G4ThreeVector field = GetField(transformedPosition, t);
      G4ThreeVector transformedField = transform * (HepGeom::Vector3D<G4double>)field;
      return transformedField;
    }
  else
    {return GetField(position,t);}
}

void BDSFieldE::GetFieldValue(const G4double point[4],
			      G4double* field) const
{
  G4ThreeVector fieldValue = GetFieldTransformed(G4ThreeVector(point[0], point[1],
							       point[2]), point[3]);
  field[0] = 0;             // B_x
  field[1] = 0;             // B_y
  field[2] = 0;             // B_z
  field[3] = fieldValue[0]; // E_x
  field[4] = fieldValue[1]; // E_y
  field[5] = fieldValue[2]; // E_z
}

void BDSFieldE::SetTransform(const G4Transform3D& transformIn)
{
  transform = transformIn;
  inverseTransform = transformIn.inverse();
}
