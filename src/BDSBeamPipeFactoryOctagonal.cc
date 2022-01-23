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
#include "BDSBeamPipeFactoryOctagonal.hh"
#include "BDSBeamPipeFactoryPoints.hh"

#include "globals.hh"                 // geant4 globals / types
#include "G4TwoVector.hh"

#include <cmath>

BDSBeamPipeFactoryOctagonal::BDSBeamPipeFactoryOctagonal()
{;}

void BDSBeamPipeFactoryOctagonal::GenerateOctagonal(std::vector<G4TwoVector>& vec,
						    G4double x1,
						    G4double y1,
						    G4double x2,
						    G4double y2)
{
  AppendPoint(vec, x1,  y2 );
  AppendPoint(vec, x1,  -y2);
  AppendPoint(vec, x2,  -y1);
  AppendPoint(vec, -x2, -y1);
  AppendPoint(vec, -x1, -y2);
  AppendPoint(vec, -x1, y2 );
  AppendPoint(vec, -x2, y1 );
  AppendPoint(vec, x2,  y1 );
}						    

BDS::FourPoints BDSBeamPipeFactoryOctagonal::ExpandOctagon(G4double aper1,
    G4double aper2,
    G4double aper3,
    G4double aper4,
    G4double distance)
{
  // All calculations done in top right quandrant
  // The angle w.r.t. the x axis for the angled part of the octagon
  G4double alpha = std::atan2(std::abs(aper2-aper4), std::abs(aper1-aper3));
  // resolve components of the beam pipe thickness (t) in each dimension
  G4double xt = distance * std::sin(alpha);
  G4double yt = distance * std::cos(alpha);

  // Calculate the deltas to the point inbetween that results in the thickness
  // of each side being exactly "distance" greater than the originals
  G4double xr = distance - xt;
  G4double dy = xr * std::tan(alpha);
  G4double dx = xt - (distance - yt)*std::tan(alpha);

  G4double a1 = aper1 + distance;
  G4double a2 = aper2 + distance;
  G4double a3 = aper3 + dx;
  G4double a4 = aper4 + dy;
  BDS::FourPoints result = {a1,a2,a3,a4};
  return result;
}

void BDSBeamPipeFactoryOctagonal::GeneratePoints(G4double aper1,
						 G4double aper2,
						 G4double aper3,
						 G4double aper4,
						 G4double beamPipeThickness,
						 G4int    /*pointsPerTwoPi*/)
{
  GenerateOctagonal(vacuumEdge, aper1, aper2, aper3, aper4);
  BDS::FourPoints ai = ExpandOctagon(aper1, aper2, aper3, aper4, lengthSafetyLarge);
  GenerateOctagonal(beamPipeInnerEdge, ai.aper1, ai.aper2, ai.aper3, ai.aper4);
  BDS::FourPoints ao = ExpandOctagon(aper1, aper2, aper3, aper4, lengthSafetyLarge + beamPipeThickness);
  GenerateOctagonal(beamPipeOuterEdge, ao.aper1, ao.aper2, ao.aper3, ao.aper4);
  BDS::FourPoints ac = ExpandOctagon(aper1, aper2, aper3, aper4, beamPipeThickness + 2*lengthSafetyLarge);
  GenerateOctagonal(containerEdge, ac.aper1, ac.aper2, ac.aper3, ac.aper4);
  BDS::FourPoints acs = ExpandOctagon(aper1, aper2, aper3, aper4, beamPipeThickness + 3*lengthSafetyLarge);
  GenerateOctagonal(containerSubtractionEdge, acs.aper1, acs.aper2, acs.aper3, acs.aper4);

  extentX = acs.aper1;
  extentY = acs.aper2;
}

G4double BDSBeamPipeFactoryOctagonal::CalculateIntersectionRadius(G4double aper1,
								  G4double aper2,
								  G4double aper3,
								  G4double aper4,
								  G4double beamPipeThickness)
{
  // We calculate the expanded aper1234 parameters corresponding to the outermost octagon
  // this has to be the same as the last ExpandOctagon in GeneratePoints()
  BDS::FourPoints acs = ExpandOctagon(aper1, aper2, aper3, aper4, beamPipeThickness + 2*lengthSafetyLarge);

  // depending on the user supplied parameters, choose the largest radius from the origin,
  // and add the beam pipe thickness plus some margin
  G4double maxXY = std::max(acs.aper1, acs.aper2);
  G4double r1 = std::hypot(acs.aper1, acs.aper4);
  G4double r2 = std::hypot(acs.aper2,acs.aper3);
  G4double result = std::max({maxXY, r1,r2});
  result += lengthSafetyLarge;
  return result;
}
