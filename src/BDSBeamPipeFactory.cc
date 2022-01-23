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
#include "BDSBeamPipeFactory.hh"
#include "BDSBeamPipeFactoryBase.hh"
#include "BDSBeamPipeFactoryCircular.hh"
#include "BDSBeamPipeFactoryCircularVacuum.hh"
#include "BDSBeamPipeFactoryClicPCL.hh"
#include "BDSBeamPipeFactoryElliptical.hh"
#include "BDSBeamPipeFactoryRectangular.hh"
#include "BDSBeamPipeFactoryLHC.hh"
#include "BDSBeamPipeFactoryLHCDetailed.hh"
#include "BDSBeamPipeFactoryOctagonal.hh"
#include "BDSBeamPipeFactoryRaceTrack.hh"
#include "BDSBeamPipeFactoryRectEllipse.hh"
#include "BDSBeamPipeInfo.hh"
#include "BDSBeamPipeType.hh"
#include "BDSDebug.hh"

#include "globals.hh"                        // geant4 globals / types

BDSBeamPipeFactory* BDSBeamPipeFactory::instance = nullptr;

BDSBeamPipeFactory* BDSBeamPipeFactory::Instance()
{
  if (!instance)
    {instance = new BDSBeamPipeFactory();}
  return instance;
}

BDSBeamPipeFactory::BDSBeamPipeFactory()
{
  circular       = new BDSBeamPipeFactoryCircular();
  elliptical     = new BDSBeamPipeFactoryElliptical();
  rectangular    = new BDSBeamPipeFactoryRectangular();
  lhc            = new BDSBeamPipeFactoryLHC();
  lhcdetailed    = new BDSBeamPipeFactoryLHCDetailed();
  rectellipse    = new BDSBeamPipeFactoryRectEllipse();
  racetrack      = new BDSBeamPipeFactoryRaceTrack();
  octagonal      = new BDSBeamPipeFactoryOctagonal();
  circularvacuum = new BDSBeamPipeFactoryCircularVacuum();
  clicpcl        = new BDSBeamPipeFactoryClicPCL();
}

BDSBeamPipeFactory::~BDSBeamPipeFactory()
{
  delete circular;
  delete elliptical;
  delete rectangular;
  delete lhc;
  delete lhcdetailed;
  delete rectellipse;
  delete racetrack;
  delete octagonal;
  delete circularvacuum;
  delete clicpcl;
  instance = nullptr;
}

BDSBeamPipeFactoryBase* BDSBeamPipeFactory::GetAppropriateFactory(BDSBeamPipeType type)
{
  switch(type.underlying())
    {
    case BDSBeamPipeType::circular:
      {return circular; break;}
    case BDSBeamPipeType::elliptical:
      {return elliptical; break;}
    case BDSBeamPipeType::rectangular:
      {return rectangular; break;}
    case BDSBeamPipeType::lhc:
      {return lhc; break;}
    case BDSBeamPipeType::lhcdetailed:
      {return lhcdetailed; break;}
    case BDSBeamPipeType::rectellipse:
      {return rectellipse; break;}
    case BDSBeamPipeType::racetrack:
      {return racetrack; break;}
    case BDSBeamPipeType::octagonal:
      {return octagonal; break;}
    case BDSBeamPipeType::circularvacuum:
      {return circularvacuum; break;}
    case BDSBeamPipeType::clicpcl:
      {return clicpcl; break;}
    default:
#ifdef BDSDEBUG
      G4cout << __METHOD_NAME__ << "unknown type \"" << type << "\" - circular beampipe factory by default" << G4endl;
#endif
      return circular;
      break;
    }
}

BDSBeamPipe* BDSBeamPipeFactory::CreateBeamPipe(G4String         name,
						G4double         length,
						BDSBeamPipeInfo* bpi)
{
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "using beam pipe information" << G4endl;
#endif
  if ((bpi->inputFaceNormal.z() > -1) || (bpi->outputFaceNormal.z() < 1))
    {
      return CreateBeamPipe(bpi->beamPipeType,
			    name,
			    length,
			    bpi->inputFaceNormal,
			    bpi->outputFaceNormal,
			    bpi->aper1,
			    bpi->aper2,
			    bpi->aper3,
			    bpi->aper4,
			    bpi->vacuumMaterial,
			    bpi->beamPipeThickness,
			    bpi->beamPipeMaterial);
    }
  else
    {
      return CreateBeamPipe(bpi->beamPipeType,
			    name,
			    length,
			    bpi->aper1,
			    bpi->aper2,
			    bpi->aper3,
			    bpi->aper4,
			    bpi->vacuumMaterial,
			    bpi->beamPipeThickness,
			    bpi->beamPipeMaterial);
    }
}
  
BDSBeamPipe* BDSBeamPipeFactory::CreateBeamPipe(BDSBeamPipeType beamPipeType,
						G4String        name,
						G4double        length,
						G4double        aper1,
						G4double        aper2,
						G4double        aper3,
						G4double        aper4,
						G4Material*     vacuumMaterial,
						G4double        beamPipeThickness,
						G4Material*     beamPipeMaterial)
{
  BDSBeamPipeFactoryBase* factory = GetAppropriateFactory(beamPipeType);
  return factory->CreateBeamPipe(name,length,aper1,aper2,aper3,aper4,
				 vacuumMaterial,beamPipeThickness,beamPipeMaterial);
}

BDSBeamPipe*  BDSBeamPipeFactory::CreateBeamPipe(BDSBeamPipeType beamPipeType,
						 G4String        name,
						 G4double        length,
						 G4ThreeVector   inputFaceNormal,
						 G4ThreeVector   outputFaceNormal,
						 G4double        aper1,
						 G4double        aper2,
						 G4double        aper3,
						 G4double        aper4,
						 G4Material*     vacuumMaterial,
						 G4double        beamPipeThickness,
						 G4Material*     beamPipeMaterial)
{
  BDSBeamPipeFactoryBase* factory = GetAppropriateFactory(beamPipeType);
  return factory->CreateBeamPipe(name,length,inputFaceNormal,outputFaceNormal,aper1,
				 aper2,aper3,aper4,vacuumMaterial,beamPipeThickness,
				 beamPipeMaterial);
}
