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
#ifndef BDSFIELDESINUSOID_H
#define BDSFIELDESINUSOID_H

#include "BDSFieldE.hh"

#include "globals.hh" // geant4 types / globals
#include "G4ThreeVector.hh"

class BDSCavityInfo;
class BDSMagnetStrength;

/**
 * @brief A sinusoidal electric (only) field that doesn't vary with position. Uses cosine.
 * 
 * The default phase is 0 so cosine is used to give acceleration by default.
 * 
 * @author Laurie Nevay
 */

class BDSFieldESinusoid: public BDSFieldE
{
public:
  /// Construct with a BDSMagnetStrength instance for convenience.
  BDSFieldESinusoid(BDSMagnetStrength const* strength,
		    G4double                 brho);

  /// Construct from E amplitude, frequency (G4Units) and phase.
  BDSFieldESinusoid(G4double eFieldAmplitude,
		    G4double frequencyIn,
		    G4double phaseOffsetIn);

  virtual ~BDSFieldESinusoid(){;}

  /// Accessor for field value.
  virtual G4ThreeVector GetField(const G4ThreeVector& position,
				 const G4double       t) const;

protected:
  /// Amplitude of electric field in V/m.
  G4double eField;
  
private:
  /// Private default constructor to force use of supplied one.
  BDSFieldESinusoid() = delete;
  
  /// Angular frequency of field.
  G4double angularFrequency;

  /// Phase in radians.
  G4double phase;
};

#endif
