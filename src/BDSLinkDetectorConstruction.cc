/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2020.

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
#include "BDSBeamline.hh"
#include "BDSBeamlineElement.hh"
#include "BDSComponentFactory.hh"
#include "BDSException.hh"
#include "BDSExtent.hh"
#include "BDSExtentGlobal.hh"
#include "BDSGlobalConstants.hh"
#include "BDSLinkComponent.hh"
#include "BDSLinkDetectorConstruction.hh"
#include "BDSLinkOpaqueBox.hh"
#include "BDSLinkRegistry.hh"
#include "BDSMaterials.hh"
#include "BDSSDManager.hh"
#include "BDSParser.hh"

#include "parser/element.h"
#include "parser/elementtype.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4VisAttributes.hh"

#include <set>
#include <vector>

class BDSParticleDefinition;

BDSLinkDetectorConstruction::BDSLinkDetectorConstruction():
  worldSolid(nullptr),
  worldPV(nullptr),
  linkBeamline(nullptr),
  linkRegistry(nullptr),
  designParticle(nullptr)
{
  linkRegistry = new BDSLinkRegistry();
  BDSSDManager::Instance()->SetLinkRegistry(linkRegistry);
}

BDSLinkDetectorConstruction::~BDSLinkDetectorConstruction()
{
  delete linkBeamline;
  delete linkRegistry;
}

G4VPhysicalVolume* BDSLinkDetectorConstruction::Construct()
{
  BDSGlobalConstants* globalConstants = BDSGlobalConstants::Instance();

  auto componentFactory = new BDSComponentFactory(designParticle);
  auto beamline = BDSParser::Instance()->GetBeamline();

  std::vector<BDSLinkOpaqueBox*> opaqueBoxes = {};
  linkBeamline = new BDSBeamline();

  for (auto elementIt = beamline.begin(); elementIt != beamline.end(); ++elementIt)
    {
      GMAD::ElementType eType = elementIt->type;

      if (eType == GMAD::ElementType::_LINE || eType == GMAD::ElementType::_REV_LINE)
        {continue;}
      
      std::set<GMAD::ElementType> acceptedTypes = {GMAD::ElementType::_ECOL,
						   GMAD::ElementType::_RCOL,
						   GMAD::ElementType::_JCOL,
						   GMAD::ElementType::_CRYSTALCOL,
						   GMAD::ElementType::_ELEMENT};
      auto search = acceptedTypes.find(eType);
      if (search == acceptedTypes.end())
	{throw BDSException(G4String("Unsupported element type for link = " + GMAD::typestr(eType)));}

      // Only need first argument, the rest pertain to beamlines.
      BDSAcceleratorComponent* component = componentFactory->CreateComponent(&(*elementIt),
									     nullptr,
									     nullptr,
									     0);

      BDSLinkOpaqueBox* opaqueBox = new BDSLinkOpaqueBox(component,
                                                         elementIt->tilt,
                                                         component->GetExtent().MaximumAbsTransverse());

      opaqueBoxes.push_back(opaqueBox);

      BDSLinkComponent* comp = new BDSLinkComponent(opaqueBox->GetName(),
							opaqueBox,
							opaqueBox->GetExtent().DZ());
      
      linkBeamline->AddComponent(comp);
    }

  // update world extents and world solid
  UpdateWorldSolid();
  
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldSolid,
						 BDSMaterials::Instance()->GetMaterial("G4_Galactic"),
						 "world_lv");

  G4VisAttributes* debugWorldVis = new G4VisAttributes(*(BDSGlobalConstants::Instance()->ContainerVisAttr()));
  debugWorldVis->SetForceWireframe(true);//just wireframe so we can see inside it
  worldLV->SetVisAttributes(debugWorldVis);
  worldLV->SetUserLimits(globalConstants->DefaultUserLimits());

  worldPV = new G4PVPlacement(nullptr,
				   G4ThreeVector(),
				   worldLV,
				   "world_pv",
				   nullptr,
				   false,
				   0,
				   true);

  // place any defined link elements in input
  for (auto element : *linkBeamline)
    {PlaceOneComponent(element);}

  delete componentFactory;

  return worldPV;
}

void BDSLinkDetectorConstruction::AddLinkCollimator(const std::string& collimatorName,
						    const std::string& materialName,
						    G4double length,
						    G4double aperture,
						    G4double rotation,
						    G4double xOffset,
						    G4double yOffset)
{
  // build component
  GMAD::Element el = GMAD::Element();
  el.type     = GMAD::ElementType::_JCOL;
  el.name     = collimatorName;
  el.material = materialName;
  el.l        = length;
  el.aper1    = aperture;
  el.tilt     = rotation;
  el.offsetX  = xOffset;
  el.offsetY  = yOffset;
  
  auto componentFactory = new BDSComponentFactory(designParticle);
  BDSAcceleratorComponent* component = componentFactory->CreateComponent(&el,
									 nullptr,
									 nullptr,
									 0); 

  // wrap in box 
  BDSLinkOpaqueBox* opaqueBox = new BDSLinkOpaqueBox(component,
						     el.tilt,
						     component->GetExtent().MaximumAbsTransverse());

  // add to beam line
  BDSLinkComponent* comp = new BDSLinkComponent(opaqueBox->GetName(),
						opaqueBox,
						opaqueBox->GetExtent().DZ());
  linkBeamline->AddComponent(comp);

  // update world extents and world solid
  UpdateWorldSolid();

  // place that one element
  PlaceOneComponent(linkBeamline->back());
}

void BDSLinkDetectorConstruction::UpdateWorldSolid()
{
  BDSExtentGlobal we = linkBeamline->GetExtentGlobal();
  we.ExpandToEncompass(BDSExtentGlobal(BDSExtent(10*CLHEP::m, 10*CLHEP::m, 10*CLHEP::m))); // minimum size
  G4ThreeVector worldExtentAbs = we.GetMaximumExtentAbsolute();
  worldExtentAbs *= 1.2;

  if (!worldSolid)
    {
      worldSolid = new G4Box("world_solid",
                             worldExtentAbs.x(),
                             worldExtentAbs.y(),
                             worldExtentAbs.z());
    }
  else
    {
      worldSolid->SetXHalfLength(worldExtentAbs.x());
      worldSolid->SetYHalfLength(worldExtentAbs.y());
      worldSolid->SetZHalfLength(worldExtentAbs.z());
    }
  worldExtent = BDSExtent(worldExtentAbs);
}

void BDSLinkDetectorConstruction::PlaceOneComponent(const BDSBeamlineElement* element)
{
  G4String placementName = element->GetPlacementName() + "_pv";
  G4Transform3D* placementTransform = element->GetPlacementTransform();
  G4int copyNumber = element->GetCopyNo();
  // auto pv =
  new G4PVPlacement(*placementTransform,
                    placementName,
                    element->GetContainerLogicalVolume(),
                    worldPV,
                    false,
                    copyNumber,
                    true);

  auto lc = dynamic_cast<BDSLinkComponent*>(element->GetAcceleratorComponent());
  if (!lc)
    {return;}
  BDSLinkOpaqueBox* el = lc->Component();
  G4Transform3D elCentreToStart = el->TransformToStart();
  G4Transform3D globalToStart = elCentreToStart * (*placementTransform);
  linkRegistry->Register(el, globalToStart);
}
