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
#include "BDSFieldMagSolenoidSheet.hh"
#include "BDSMagnetStrength.hh"
#include "BDSUtilities.hh"

#include "G4ThreeVector.hh"
#include "G4Types.hh"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <algorithm>
#include <cmath>

BDSFieldMagSolenoidSheet::BDSFieldMagSolenoidSheet(BDSMagnetStrength const* strength,
                                                   G4double radiusIn):
  BDSFieldMagSolenoidSheet((*strength)["length"], radiusIn, (*strength)["field"])
{;}

BDSFieldMagSolenoidSheet::BDSFieldMagSolenoidSheet(G4double fullLength,
						   G4double sheetRadius,
						   G4double B0In):
  a(sheetRadius),
  halfLength(0.5*fullLength),
  B0(B0In),
  spatialLimit(std::min(1e-5*sheetRadius, 1e-5*fullLength)),
  normalisation(1.0)
{
  finiteStrength = BDS::IsFinite(B0In);
  
  // The field inside the current cylinder is actually slightly parabolic in rho.
  // The equation for the field takes B0 as the peak magnetic field at the current
  // cylinder sheet. So we evaluate it here then normalise. ~<1% adjustment in magnitude.
  G4double testBz = OnAxisBz(halfLength, -halfLength);
  normalisation = B0 / testBz;
}

G4ThreeVector BDSFieldMagSolenoidSheet::GetField(const G4ThreeVector& position,
                                                 const G4double       /*t*/) const
{
  G4double z = position.z();
  G4double rho = position.perp();
  G4double phi = position.phi(); // angle about z axis
  
  // check if close to current source - function not well-behaved at exactly the rho of
  // the current source or at the boundary of +- halfLength
  if (std::abs(std::abs(z) - halfLength) < spatialLimit && (rho < a+2*spatialLimit))
    {return G4ThreeVector();} // close to +-z edge of cylinder and inside the radius
  else if (std::abs(rho - a) < spatialLimit && (std::abs(z) < halfLength+2*spatialLimit))
    {return G4ThreeVector();} // close to radius and inside +- z
  
  G4double zp = z + halfLength;
  G4double zm = z - halfLength;
  
  G4double Brho = 0;
  G4double Bz   = 0;
  
  // approximation for on-axis
  if (std::abs(rho) < spatialLimit)
    {Bz = OnAxisBz(zp, zm);}
  else
    {
      G4double zpSq = zp*zp;
      G4double zmSq = zm*zm;
      
      G4double rhoPlusA = rho + a;
      G4double rhoPlusASq = rhoPlusA * rhoPlusA;
      G4double aMinusRho = a - rho;
      G4double aMinusRhoSq = aMinusRho*aMinusRho;
      
      G4double denominatorP = std::sqrt(zpSq + rhoPlusASq);
      G4double denominatorM = std::sqrt(zmSq + rhoPlusASq);
      
      G4double alphap = a / denominatorP;
      G4double alpham = a / denominatorM;
      
      G4double betap = zp / denominatorP;
      G4double betam = zm / denominatorM;
      
      G4double gamma = (a - rho) / (rhoPlusA);
      G4double gammaSq = gamma * gamma;

      G4double kp = std::sqrt(zpSq + aMinusRhoSq) / denominatorP;
      G4double km = std::sqrt(zmSq + aMinusRhoSq) / denominatorM;
      
      Brho = B0 * (alphap * CEL(kp, 1, 1, -1) - alpham * CEL(km, 1, 1, -1)) / CLHEP::pi;
      Bz = ((B0 * a) / (rhoPlusA)) * (betap * CEL(kp, gammaSq, 1, gamma) - betam * CEL(km, gammaSq, 1, gamma)) / CLHEP::pi;
      // technically possible for integral to return nan, so protect against it and default to B0 along z
      if (std::isnan(Brho))
	{Brho = 0;}
      if (std::isnan(Bz))
	{Bz = B0;}
    }
  // we have to be consistent with the phi we calculated at the beginning,
  // so unit rho is in the x direction.
  G4ThreeVector result = G4ThreeVector(Brho,0,Bz) * normalisation;
  result = result.rotateZ(phi);
  return result;
}

G4double BDSFieldMagSolenoidSheet::CEL(G4double kc,
                                       G4double p,
                                       G4double c,
                                       G4double s,
                                       G4int nIterationLimit)
{
  if (!BDS::IsFinite(kc))
    {return NAN;}
  
  G4double errtol = 0.000001;
  G4double k = std::abs(kc);
  G4double pp = p;
  G4double cc = c;
  G4double ss = s;
  G4double em = 1.0;
  if (p > 0)
    {
      pp = std::sqrt(p);
      ss = s/pp;
    }
  else
    {
      G4double f = kc * kc;
      G4double q = 1.0 - f;
      G4double g = 1.0 - pp;
      f = f - pp;
      q = q * (ss - c * pp);
      pp = std::sqrt(f / g);
      cc = (c - ss) / g;
      ss = -q / (g * g * pp) + cc * pp;
    }
  
  G4double f = cc;
  cc = cc + ss/pp;
  G4double g = k/pp;
  ss = 2*(ss + f*g);
  pp = g + pp;
  g = em;
  em = k + em;
  G4double kk = k;
  G4int nLoop = 0;
  while ( (std::abs(g-k) > g*errtol) && nLoop < nIterationLimit)
    {
      k = 2*std::sqrt(kk);
      kk = k*em;
      f = cc;
      cc = cc + ss/pp;
      g = kk / pp;
      ss = 2*(ss + f*g);
      pp = g + pp;
      g = em;
      em = k + em;
      nLoop++;
    }
  G4double result = CLHEP::halfpi*(ss + cc*em)/( em*(em + pp) );
  return result;
}

G4double BDSFieldMagSolenoidSheet::OnAxisBz(G4double zp,
                                            G4double zm) const
{
  G4double f1 = zp / std::sqrt( zp*zp + a*a );
  G4double f2 = zm / std::sqrt( zm*zm + a*a );
  G4double Bz = 0.5*B0 * (f1 - f2);
  return Bz;
}
