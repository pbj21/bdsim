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
#include "BDSAcceleratorModel.hh"
#include "BDSBeamline.hh"
#include "BDSBunch.hh"
#include "BDSDebug.hh"
#include "BDSException.hh"
#include "BDSGlobalConstants.hh"
#include "BDSIonDefinition.hh"
#include "BDSParticleCoords.hh"
#include "BDSParticleCoordsFull.hh"
#include "BDSParticleCoordsFullGlobal.hh"
#include "BDSParticleDefinition.hh"
#include "BDSPhysicsUtilities.hh"
#include "BDSUtilities.hh"

#include "parser/beam.h"

#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4TwoVector.hh"
#include "G4Version.hh"

#include "CLHEP/Geometry/Point3D.h"

#include <cmath>
#include <limits>
#include <set>
#include <string>


BDSBunch::BDSBunch():
  BDSBunch("reference")
{;}

BDSBunch::BDSBunch(const G4String& nameIn):
  name(nameIn),
  X0(0.0), Y0(0.0), Z0(0.0), S0(0.0), T0(0.0), 
  Xp0(0.0), Yp0(0.0), Zp0(0.0), E0(0.0), P0(0.0),
  tilt(0.0),
  sigmaT(0.0), sigmaP(0.0), sigmaE(0.0), sigmaEk(0.0),
  useCurvilinear(false),
  particleDefinition(nullptr),
  particleDefinitionHasBeenUpdated(false),
  finiteTilt(false),
  finiteSigmaE(true),
  finiteSigmaT(true),
  generatePrimariesOnly(false),
  beamlineTransform(G4Transform3D()),
  beamlineS(0),
  mass2(0.0),
  beamline(nullptr)
{;}

BDSBunch::~BDSBunch()
{
  delete particleDefinition;
}

void BDSBunch::SetOptions(const BDSParticleDefinition* beamParticle,
			  const GMAD::Beam& beam,
			  const BDSBunchType& /*distrType*/,
			  G4Transform3D beamlineTransformIn,
			  G4double beamlineSIn)
{
  particleDefinition = new BDSParticleDefinition(*beamParticle); // copy it so this instance owns it

  // back the starting point up by length safety to avoid starting on a boundary
  G4ThreeVector unitZBeamline = G4ThreeVector(0,0,-1).transform(beamlineTransformIn.getRotation());
  G4ThreeVector translation   = BDSGlobalConstants::Instance()->LengthSafety() * unitZBeamline;
  beamlineTransform = G4Transform3D(beamlineTransformIn.getRotation(), beamlineTransformIn.getTranslation()+translation);

  beamlineS = beamlineSIn;

  X0     = beam.X0 * CLHEP::m;
  Y0     = beam.Y0 * CLHEP::m;
  Z0     = beam.Z0 * CLHEP::m;
  S0     = beam.S0 * CLHEP::m;
  T0     = beam.T0 * CLHEP::s;
  Xp0    = beam.Xp0 * CLHEP::rad;
  Yp0    = beam.Yp0 * CLHEP::rad;
  E0     = particleDefinition->TotalEnergy(); // already calculated and set earlier depending on available parameters
  P0     = particleDefinition->Momentum();
  tilt   = beam.tilt * CLHEP::rad;
  sigmaT = beam.sigmaT;
  sigmaP = beam.sigmaP;
  sigmaE = beam.sigmaE;
  sigmaEk = beam.sigmaEk;

  finiteTilt   = BDS::IsFinite(tilt);
  finiteSigmaE = BDS::IsFinite(sigmaE);
  finiteSigmaT = BDS::IsFinite(sigmaT);
  G4bool finiteSigmaP = BDS::IsFinite(sigmaP);
  G4bool finiteSigmaEk = BDS::IsFinite(sigmaEk);

  std::set<std::string> keysDesign = {"sigmaE", "sigmaEk", "sigmaP"};
  G4int nSetDesign = BDS::NBeamParametersSet(beam, keysDesign);
  BDS::ConflictingParametersSet(beam, keysDesign, nSetDesign, false, "GeV");// warn only if too many set
  if (finiteSigmaE)
    {
      sigmaP = (1./std::pow(beamParticle->Beta(),2)) * sigmaE; // dE/E = (beta^2) dP/P
      sigmaEk = (beamParticle->TotalEnergy() / beamParticle->KineticEnergy()) * sigmaE;
    }
  else if (finiteSigmaP)
    {
      sigmaE = std::pow(beamParticle->Beta(),2) * sigmaP;
      sigmaEk = (beamParticle->TotalEnergy() / beamParticle->KineticEnergy()) * sigmaE;
    }
  else if (finiteSigmaEk)
    {
      sigmaE = sigmaEk * (beamParticle->KineticEnergy() / beamParticle->TotalEnergy());
      sigmaP = (1./std::pow(beamParticle->Beta(),2)) * sigmaE; // dE/E = (beta^2) dP/P
    }
  // else they'll all be 0 - no need for a calculation
  
  finiteSigmaE = finiteSigmaE || finiteSigmaP || finiteSigmaEk; // finiteSigmaE used to know whether any variation in other classes
  if (finiteSigmaE)
    {
      G4cout << "Beam> sigmaP:    " << sigmaP  << G4endl;
      G4cout << "Beam> sigmaE:    " << sigmaE  << G4endl;
      G4cout << "Beam> sigmaEk:   " << sigmaEk << G4endl;
    }

  Zp0 = CalculateZp(Xp0,Yp0,beam.Zp0);

  if (S0 > beamlineS)
    {
#ifdef BDSDEBUG
      G4cout << __METHOD_NAME__ << "using curvilinear transform" << G4endl;
#endif
      if (BDS::IsFinite(Z0))
	{throw BDSException(__METHOD_NAME__, "both Z0 and S0 are defined - please define only one!");}
      useCurvilinear = true;
    } 
}

void BDSBunch::SetEmittances(const BDSParticleDefinition* beamParticle,
			     const GMAD::Beam& beam,
			     G4double&         emittGeometricX,
			     G4double&         emittGeometricY,
			     G4double&         emittNormalisedX,
			     G4double&         emittNormalisedY)
{
  std::set<std::string> keysDesignX = {"emitx", "emitnx"};
  G4int nSetDesignX = BDS::NBeamParametersSet(beam, keysDesignX);
  BDS::ConflictingParametersSet(beam, keysDesignX, nSetDesignX);
  if (BDS::IsFinite(beam.emitNX))
    {
      emittNormalisedX = G4double(beam.emitNX);
      emittGeometricX  = G4double(beam.emitNX) / beamParticle->Gamma();
    }
  else
    {
      emittGeometricX  = G4double(beam.emitx);
      emittNormalisedX = G4double(beam.emitx) * beamParticle->Gamma();
    }
  
  std::set<std::string> keysDesignY = {"emity", "emitny"};
  G4int nSetDesignY = BDS::NBeamParametersSet(beam, keysDesignY);
  BDS::ConflictingParametersSet(beam, keysDesignY, nSetDesignY);
  if (BDS::IsFinite(beam.emitNY))
    {
      emittNormalisedY = G4double(beam.emitNY);
      emittGeometricY  = G4double(beam.emitNY) / beamParticle->Gamma();}
  else
    {
      emittGeometricY  = G4double(beam.emity);
      emittNormalisedY = G4double(beam.emity) * beamParticle->Gamma();
    }

  G4cout << __METHOD_NAME__ << "Geometric (x): " << emittGeometricX
	 << ", Normalised (x): " << emittNormalisedX << G4endl;
  G4cout << __METHOD_NAME__ << "Geometric (y): " << emittGeometricY
	 << ", Normalised (y): " << emittNormalisedY << G4endl;
}

void BDSBunch::CheckParameters()
{
  if (sigmaE < 0)
    {throw BDSException(__METHOD_NAME__, "sigmaE " + std::to_string(sigmaE) + " < 0!");}
  if (sigmaT < 0)
    {throw BDSException(__METHOD_NAME__, "sigmaT " + std::to_string(sigmaT) + " < 0!");}
}

void BDSBunch::Initialise()
{;}

BDSParticleCoordsFullGlobal BDSBunch::GetNextParticleValid(G4int maxTries)
{
  particleDefinitionHasBeenUpdated = false; // reset flag for this call
  // use a separate flag to record whether the particle definitions has
  // been updated as subsequent calls to GetNextParticle may reset it to
  // false but it was updated in the first call
  G4bool flag = false;

  // continue generating particles until positive finite kinetic energy.
  G4int n = 0;
  BDSParticleCoordsFullGlobal coords;
  while (n < maxTries) // prevent infinite loops
    {
      ++n;
      coords = GetNextParticle();
      flag = flag || particleDefinitionHasBeenUpdated;
      
      // ensure total energy is greater than the rest mass
      if ((coords.local.totalEnergy - particleDefinition->Mass()) > 0)
	{break;}
    }
  if (n >= maxTries)
    {throw BDSException(__METHOD_NAME__, "unable to generate coordinates above rest mass after 100 attempts.");}

  particleDefinitionHasBeenUpdated = flag;
  return coords;
}

BDSParticleCoordsFullGlobal BDSBunch::GetNextParticle()
{
  particleDefinitionHasBeenUpdated = false; // reset flag
  BDSParticleCoordsFull local = GetNextParticleLocal();
  if (finiteTilt)
    {ApplyTilt(local);}
  BDSParticleCoordsFullGlobal all = ApplyTransform(local);
  return all;
}

BDSParticleCoordsFull BDSBunch::GetNextParticleLocal()
{
  BDSParticleCoordsFull local(X0,  Y0,  Z0,
			      Xp0, Yp0, Zp0,
			      T0, S0, E0, /*weight=*/1.0);
  return local;
}

void BDSBunch::BeginOfRunAction(G4int /*numberOfEvents*/)
{;}

void BDSBunch::SetGeneratePrimariesOnly(G4bool generatePrimariesOnlyIn)
{generatePrimariesOnly = generatePrimariesOnlyIn;}

BDSParticleCoordsFullGlobal BDSBunch::ApplyTransform(const BDSParticleCoordsFull& localIn) const
{
  if (useCurvilinear) // i.e. S0 is finite - use beam line
    {return ApplyCurvilinearTransform(localIn);}
  else // just general beam line offset
    {return BDSParticleCoordsFullGlobal(localIn,(BDSParticleCoords)localIn.ApplyTransform(beamlineTransform));}
}

void BDSBunch::ApplyTilt(BDSParticleCoordsFull& localIn) const
{
  G4TwoVector xy(localIn.x, localIn.y);
  G4TwoVector xpyp(localIn.xp, localIn.yp);
  xy.rotate(tilt);
  xpyp.rotate(tilt);
  localIn.x = xy.x();
  localIn.y = xy.y();
  localIn.xp = xpyp.x();
  localIn.yp = xpyp.y();
}

BDSParticleCoordsFullGlobal BDSBunch::ApplyCurvilinearTransform(const BDSParticleCoordsFull& localIn) const
{
  if (generatePrimariesOnly) // no beam line built so no possible transform
    {return BDSParticleCoordsFullGlobal(localIn, (BDSParticleCoords)localIn);}

  if (!beamline)
    {// initialise cache of beam line pointer
      beamline = BDSAcceleratorModel::Instance()->BeamlineMain();
      if (!beamline)
	{throw BDSException(__METHOD_NAME__, "no beamline constructed!");}
    }

  // 'c' for curvilinear
  G4int beamlineIndex = 0;
  G4double S = S0 + localIn.z;
  if (S < 0)
    {throw BDSException(__METHOD_NAME__, "Negative S detected for particle.");}
  
  G4Transform3D cTrans = beamline->GetGlobalEuclideanTransform(S,
							       localIn.x,
							       localIn.y,
							       &beamlineIndex);
  // rotate the momentum vector
  G4ThreeVector cMom = G4ThreeVector(localIn.xp, localIn.yp, localIn.zp).transform(cTrans.getRotation());
  // translation contains displacement from origin already - including any local offset
  G4ThreeVector cPos = cTrans.getTranslation();

  BDSParticleCoords global = BDSParticleCoords(cPos.x(), cPos.y(), cPos.z(),
					       cMom.x(), cMom.y(), cMom.z(),
					       localIn.T);

  BDSParticleCoordsFullGlobal result = BDSParticleCoordsFullGlobal(localIn, global);
  result.beamlineIndex = beamlineIndex;
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << result << G4endl;
#endif
  return result;
}

G4double BDSBunch::CalculateZp(G4double xp, G4double yp, G4double Zp0In)
{
  G4double zp;
  G4double transMom = std::pow(xp, 2) + std::pow(yp, 2);

  if (transMom > (1 - std::numeric_limits<double>::epsilon()))
    {throw BDSException(__METHOD_NAME__, "xp, yp too large, xp: " + std::to_string(xp) + " yp: " + std::to_string(yp));}
  if (Zp0In < 0)
    {zp = -std::sqrt(1.0 - transMom);}
  else
    {zp = std::sqrt(1.0 - transMom);}

  return zp;
}

void BDSBunch::UpdateIonDefinition()
{
  if (!particleDefinition->IsAnIon())
    {return;}
  
  G4IonTable* ionTable = G4ParticleTable::GetParticleTable()->GetIonTable();
  BDSIonDefinition* ionDefinition = particleDefinition->IonDefinition();
  G4ParticleDefinition* ionParticleDef = ionTable->GetIon(ionDefinition->Z(),
							  ionDefinition->A(),
							  ionDefinition->ExcitationEnergy());
  particleDefinition->UpdateG4ParticleDefinition(ionParticleDef);
  // Note we don't need to take care of electrons here. These are automatically
  // allocated by Geant4 when it converts the primary vertex to a dynamic particle
  // (in the process of constructing a track from it) (done in G4PrimaryTransformer)
  // this relies on the charge being set correctly - Geant4 detects this isn't the same
  // as Z and adds electrons accordingly.
#if G4VERSION_NUMBER > 1049
  // in the case of ions the particle definition is only available now
  // fix the looping thresholds now it's available
  BDS::FixGeant105ThreshholdsForParticle(ionParticleDef);
#endif
}
