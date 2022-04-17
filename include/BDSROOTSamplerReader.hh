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
#ifndef BDSROOTSAMPLERREADER_H
#define BDSROOTSAMPLERREADER_H

#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPrimaryGenerator.hh"

#include <vector>

class BDSBunchEventGenerator;
class BDSOutputLoaderSampler;
class G4Event;
class G4PrimaryParticle;
class G4PrimaryVertex;
class G4VSolid;

/**
 * @brief Loader to read a specific sampler from a BDSIM ROOT output file.
 * 
 * @author Laurie Nevay
 */

class BDSROOTSamplerReader: public G4VPrimaryGenerator
{
public:
  /// Do not require default constructor.
  BDSROOTSamplerReader() = delete;
  /// Constructor takes full distrType string including semicolon and
  /// eventgeneratorfile prefix. The filename is assumed to be correctly
  /// prefixed if a relative path already. The bunch definition is used
  /// for the reference coordinates and offset of the beam point.
  BDSROOTSamplerReader(const G4String& distrType,
		       const G4String& fileNameIn,
		       BDSBunchEventGenerator* bunchIn,
                       G4bool removeUnstableWithoutDecayIn = true,
		       G4bool warnAboutSkippedParticlesIn = true);
  virtual ~BDSROOTSamplerReader();

  /// Read the next non-empty sampler entry from the file.
  virtual void GeneratePrimaryVertex(G4Event* anEvent);

  /// Advance to the correct event number in the file for recreation.
  virtual void RecreateAdvanceToEvent(G4int eventOffset);

  struct DisplacedVertex
  {
    G4ThreeVector xyz;
    G4PrimaryParticle* vertex;
  };
  
protected:
  /// Clear the hepmcEvent object, reallocate and read a single event and fill that member.
  void ReadSingleEvent(G4long index);

  /// Conversion from HepMC::GenEvent to G4Event.
  //void HepMC2G4(const HepMC3::GenEvent* hepmcevt, G4Event* g4event);
  
  // We  have to take care for the position of primaries because
  // primary vertices outside the world volume would give rise to a G4Exception.
  virtual G4bool VertexInsideWorld(const G4ThreeVector& pos) const;
  
  void ReadPrimaryParticlesFloat(G4long index);
  void ReadPrimaryParticlesDouble(G4long index);

private:
  G4long                    currentFileEventIndex;
  G4long                    nEventsInFile;
  BDSOutputLoaderSampler*   reader;
  G4String                  fileName;
  G4String                  samplerName;
  BDSBunchEventGenerator*   bunch;
  G4bool                    removeUnstableWithoutDecay;
  G4bool                    warnAboutSkippedParticles;
  G4RotationMatrix          referenceBeamMomentumOffset;
  mutable G4VSolid*         worldSolid;
  
  /// We need to keep a note of if we find any particles at all when looping through a file
  /// so we can distinguish if the whole file had no particles, or say the last event doesn't
  /// before we loop through the file again.
  G4bool anyParticlesFoundAtAll;
  
  /// Used for transiently loading information.
  std::vector<DisplacedVertex> vertices;
  
  std::vector<G4PrimaryVertex*> currentVertices;
};

#endif
