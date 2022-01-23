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
#ifdef USE_HEPMC3

#include "BDSBunchEventGenerator.hh"
#include "BDSDebug.hh"
#include "BDSEventGeneratorFileType.hh"
#include "BDSException.hh"
#include "BDSHepMC3Reader.hh"
#include "BDSParticleCoords.hh"
#include "BDSParticleCoordsFull.hh"
#include "BDSParticleCoordsFullGlobal.hh"
#include "BDSPhysicalConstants.hh"
#include "BDSPrimaryVertexInformation.hh"
#include "BDSPrimaryVertexInformationV.hh"
#include "BDSUtilities.hh"

#include "G4Event.hh"
#include "G4LorentzVector.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "G4VSolid.hh"

#include "HepMC3/Attribute.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"
#ifdef USE_HEPMC3_ROOTIO
#include "HepMC3/ReaderRoot.h"
#include "HepMC3/ReaderRootTree.h"
#endif

#include "CLHEP/Units/SystemOfUnits.h"

#include "globals.hh"

#include <utility>


BDSHepMC3Reader::BDSHepMC3Reader(const G4String& distrType,
				 const G4String& fileNameIn,
				 BDSBunchEventGenerator* bunchIn,
				 G4bool removeUnstableWithoutDecayIn,
				 G4bool warnAboutSkippedParticlesIn):
  hepmcEvent(nullptr),
  reader(nullptr),
  fileName(fileNameIn),
  bunch(bunchIn),
  removeUnstableWithoutDecay(removeUnstableWithoutDecayIn),
  warnAboutSkippedParticles(warnAboutSkippedParticlesIn),
  worldSolid(nullptr)
{
  std::pair<G4String, G4String> ba = BDS::SplitOnColon(distrType); // before:after
  fileType = BDS::DetermineEventGeneratorFileType(ba.second);
  G4cout << __METHOD_NAME__ << "event generator file format to be " << fileType.ToString() << G4endl;
  referenceBeamMomentumOffset = bunch->ReferenceBeamMomentumOffset();
  OpenFile();
}

BDSHepMC3Reader::~BDSHepMC3Reader()
{
  delete hepmcEvent;
  delete reader;
}

void BDSHepMC3Reader::GeneratePrimaryVertex(G4Event* anEvent)
{
  if (!reader)
    {throw BDSException(__METHOD_NAME__, "no file reader available");}
  
  ReadSingleEvent();  
  HepMC2G4(hepmcEvent, anEvent);
}

void BDSHepMC3Reader::RecreateAdvanceToEvent(G4int eventOffset)
{
  G4cout << "BDSHepMC3Reader::RecreateAdvanceToEvent> Advancing file to event: " << eventOffset << G4endl;
  for (G4int i = 0; i < eventOffset; i++)
    {ReadSingleEvent();}
}

void BDSHepMC3Reader::OpenFile()
{
  G4cout << __METHOD_NAME__ << "Opening file: " << fileName << G4endl;
  switch (fileType.underlying())
    {
    case BDSEventGeneratorFileType::hepmc2:
      {reader = new HepMC3::ReaderAsciiHepMC2(fileName); break;}
    case BDSEventGeneratorFileType::hepmc3:
      {reader = new HepMC3::ReaderAscii(fileName); break;}
    case BDSEventGeneratorFileType::hpe:
      {reader = new HepMC3::ReaderHEPEVT(fileName); break;}
#ifdef USE_HEPMC3_ROOTIO
    case BDSEventGeneratorFileType::root:
      {reader = new HepMC3::ReaderRoot(fileName); break;}
    case BDSEventGeneratorFileType::treeroot:
      {reader = new HepMC3::ReaderRootTree(fileName); break;}
#else
    case BDSEventGeneratorFileType::root:
    case BDSEventGeneratorFileType::treeroot:
      {
	throw BDSException(__METHOD_NAME__, "HEPMC3 installation not compiled with ROOT support so can't load root file.");
	break;
      }
#endif
    case BDSEventGeneratorFileType::lhef:
      {reader = new HepMC3::ReaderLHEF(fileName); break;}
    default:
      {break;}
    }
}

void BDSHepMC3Reader::CloseFile()
{
  if (reader)
    {reader->close();}
  delete reader;
}

void BDSHepMC3Reader::ReadSingleEvent()
{
  delete hepmcEvent;
  hepmcEvent = new HepMC3::GenEvent();

  bool readEventOK = reader->read_event(*hepmcEvent);
  if (!readEventOK)
    {throw BDSException(__METHOD_NAME__, "problem with event generator file \"" + fileName + "\"");}
  if (reader->failed()) // code for end of the file
    {
      G4cout << __METHOD_NAME__ << "End of file reached. Return to beginning of file for next event." << G4endl;
      CloseFile();
      OpenFile();
      delete hepmcEvent;
      hepmcEvent = new HepMC3::GenEvent();
      readEventOK = reader->read_event(*hepmcEvent);
      if (!readEventOK)
        {throw BDSException(__METHOD_NAME__, "cannot read file \"" + fileName + "\".");}
    }
}

void BDSHepMC3Reader::HepMC2G4(const HepMC3::GenEvent* hepmcevt,
			       G4Event* g4event)
{
  BDSParticleCoordsFull centralCoords = bunch->GetNextParticleLocal();
  // do the transform for the reference particle (if any transform) and use that
  BDSParticleCoordsFullGlobal centralCoordsGlobal = bunch->ApplyTransform(centralCoords);
  G4PrimaryVertex* g4vtx = new G4PrimaryVertex(centralCoordsGlobal.global.x,
					       centralCoordsGlobal.global.y,
					       centralCoordsGlobal.global.z,
					       centralCoordsGlobal.global.T);
  
  double overallWeight = 1.0;
  if (!(hepmcevt->weights().empty()))
    {overallWeight = hepmcevt->weight();}
  std::vector<BDSPrimaryVertexInformation> vertexInfos;
  G4int nParticlesSkipped = 0;
  for (const auto& particlePtr : hepmcevt->particles())
    {
      const HepMC3::GenParticle* particle = particlePtr.get();
      if (!particle->children().empty())
	{continue;} // this particle is not at the end of the tree - ignore
      
      int pdgcode = particle->pdg_id();
      const HepMC3::FourVector& fv = particle->momentum();
      G4LorentzVector p(fv.px(), fv.py(), fv.pz(), fv.e());
      G4double px = p.x() * CLHEP::GeV;
      G4double py = p.y() * CLHEP::GeV;
      G4double pz = p.z() * CLHEP::GeV;
      G4ThreeVector originalUnitMomentum(px,py,pz);
      originalUnitMomentum = originalUnitMomentum.unit();
      G4double rp = std::hypot(originalUnitMomentum.x(), originalUnitMomentum.y());
      
      // apply any reference coordinate offsets. Copy the vector first.
      G4ThreeVector unitMomentum = originalUnitMomentum;
      unitMomentum.transform(referenceBeamMomentumOffset);
      // it's ok that this G4PrimaryParticle doesn't have the correct momentum direction as we only use it for
      // total energy and checking - it's direction is updated later based on unitMomentum with a beam line transform.
      G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, px, py, pz);

      // if the particle definition isn't found from the pdgcode in the construction
      // of G4PrimaryParticle, it means the mass, charge, etc will be wrong - don't
      // stack this particle into the vertex.
      const G4ParticleDefinition* pd = g4prim->GetParticleDefinition();
      G4bool deleteIt = !pd;
      if (pd && removeUnstableWithoutDecay)
        {deleteIt = !(pd->GetPDGStable()) && !pd->GetDecayTable();}

      if (deleteIt)
        {
#ifdef BDSDEBUG
          G4cout << __METHOD_NAME__ << "skipping particle with PDG ID: " << pdgcode << G4endl;
#endif
          delete g4prim;
          nParticlesSkipped++;
          continue;
        }

      BDSParticleCoordsFull local(centralCoords.x,
				  centralCoords.y,
				  centralCoords.z,
				  unitMomentum.x(),
				  unitMomentum.y(),
				  unitMomentum.z(),
				  centralCoords.T,
				  centralCoords.s,
				  g4prim->GetTotalEnergy(),
				  overallWeight);

      if (!bunch->AcceptParticle(local, rp, g4prim->GetKineticEnergy(), pdgcode))
	{
	  delete g4prim;
	  nParticlesSkipped++;
	  continue;
	}
      
      BDSParticleCoordsFullGlobal fullCoords = bunch->ApplyTransform(local);
      // ensure it's in the world - not done in primary generator action for event generators
      if (!VertexInsideWorld(fullCoords.global.Position()))
	{
	  delete g4prim;
	  nParticlesSkipped++;
	  continue;
	}  
      
      G4double brho     = 0;
      G4double charge   = g4prim->GetCharge();
      G4double momentum = g4prim->GetTotalMomentum();
      if (BDS::IsFinite(charge)) // else leave as 0
	{
	  brho = momentum / CLHEP::GeV / BDS::cOverGeV / charge;
	  brho *= CLHEP::tesla*CLHEP::m; // rigidity (in Geant4 units)
	}
      BDSPrimaryVertexInformation vertexInfo(fullCoords,
					     g4prim->GetTotalMomentum(),
					     g4prim->GetCharge(),
					     brho,
					     g4prim->GetMass(),
					     pdgcode);
      vertexInfos.emplace_back(vertexInfo);

      // update momentum in case of a beam line transform
      g4prim->SetMomentumDirection(G4ThreeVector(fullCoords.global.xp,
						 fullCoords.global.yp,
						 fullCoords.global.zp));
      g4vtx->SetPrimary(g4prim);
    }

  if (nParticlesSkipped > 0 && warnAboutSkippedParticles)
    {G4cout << __METHOD_NAME__ << nParticlesSkipped << " particles skipped" << G4endl;}
  g4vtx->SetUserInformation(new BDSPrimaryVertexInformationV(vertexInfos));
  
  g4event->AddPrimaryVertex(g4vtx);
}

G4bool BDSHepMC3Reader::VertexInsideWorld(const G4ThreeVector& pos) const
{
  if (!worldSolid)
    {// cache the world solid
      G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
      G4VPhysicalVolume* world = navigator->GetWorldVolume();
      worldSolid = world->GetLogicalVolume()->GetSolid();
    }
  EInside qinside = worldSolid->Inside(pos);
  return qinside == kInside;
}

#else
// insert empty function to avoid no symbols warning
void _SymbolToPreventWarningHEPMC3Reader(){;}
#endif
