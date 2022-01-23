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
#include "BDSFieldESinusoid.hh"
#include "BDSMagnetStrength.hh"
#include "BDSUtilities.hh"

#include "globals.hh"
#include "G4ThreeVector.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include <cmath>

BDSFieldESinusoid::BDSFieldESinusoid(BDSMagnetStrength const* strength,
				     G4double                 brho):
  BDSFieldESinusoid((*strength)["efield"],
		    (*strength)["frequency"],
		    (*strength)["phase"])
{
  G4int sign = BDS::Sign(brho);
  eField *= sign;
}

BDSFieldESinusoid::BDSFieldESinusoid(G4double eFieldAmplitudeIn,
				     G4double frequencyIn,
				     G4double phaseOffsetIn):
  eField(eFieldAmplitudeIn),
  angularFrequency(CLHEP::twopi*frequencyIn),
  phase(phaseOffsetIn)
{
  finiteStrength = BDS::IsFinite(eField);
}

G4ThreeVector BDSFieldESinusoid::GetField(const G4ThreeVector& /*position*/,
					  const G4double       t) const
{
  G4double eZ = eField*std::cos(angularFrequency*t + phase);
  G4ThreeVector field = G4ThreeVector(0, 0, eZ);
  return field;
}
