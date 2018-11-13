/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2018.

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
#include "BDSColours.hh"
#include "BDSDebug.hh"
#include "BDSExtent.hh"
#include "BDSGeometryComponent.hh"
#include "BDSGlobalConstants.hh"
#include "BDSMagnetOuterFactoryBase.hh"
#include "BDSMaterials.hh"

#include "globals.hh" // geant4 types / globals

#include "G4Colour.hh"
#include "G4CutTubs.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

// Note the geometry cannot scale arbitrarily with this constant. It can be
// upscaled for debugging purposes, however, the extruded solids that use
// it may fail when this is increased to approximately 1mm and with certain
// small / tight magnet shapes where the size of the magnet or components
// becomse comparable to this constant. Upscaling only for debugging purposes.
G4double const BDSMagnetOuterFactoryBase::lengthSafetyLarge = 1*CLHEP::um;

BDSMagnetOuterFactoryBase::BDSMagnetOuterFactoryBase()
{
  lengthSafety       = BDSGlobalConstants::Instance()->LengthSafety();
  checkOverlaps      = BDSGlobalConstants::Instance()->CheckOverlaps();
  visDebug           = BDSGlobalConstants::Instance()->VisDebug();
  nSegmentsPerCircle = BDSGlobalConstants::Instance()->NSegmentsPerCircle();
  sensitiveOuter     = BDSGlobalConstants::Instance()->SensitiveOuter();
  containerVisAttr   = BDSGlobalConstants::Instance()->ContainerVisAttr();
  defaultUserLimits  = BDSGlobalConstants::Instance()->DefaultUserLimits();

  // initialise variables and pointers that'll be used by the factory
  CleanUp();
}

void BDSMagnetOuterFactoryBase::CleanUp()
{
  poleSolid            = nullptr;
  yokeSolid            = nullptr;
  containerSolid       = nullptr;
  magnetContainerSolid = nullptr;
  poleLV               = nullptr;
  yokeLV               = nullptr;
  containerLV          = nullptr;
  magnetContainerLV    = nullptr;
  yokePV               = nullptr;
  outerVisAttributes   = nullptr;

  allLogicalVolumes.clear();
  allPhysicalVolumes.clear();
  allRotationMatrices.clear();
  allSolids.clear();
  allVisAttributes.clear();

  magnetContainer = nullptr;

  inputFaceNormal  = G4ThreeVector(0,0,-1);
  outputFaceNormal = G4ThreeVector(0,0, 1);
}

void BDSMagnetOuterFactoryBase::CreateLogicalVolumes(G4String    name,
						     G4Colour*   colour,
						     G4Material* outerMaterial)
{
  if (poleSolid)
    {
      poleLV = new G4LogicalVolume(poleSolid,
				   outerMaterial,
				   name + "_pole_lv");
    }
  
  yokeLV = new G4LogicalVolume(yokeSolid,
			       outerMaterial,
			       name + "_yoke_lv");

  G4Material* worldMaterial = BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->WorldMaterial());
  containerLV = new G4LogicalVolume(containerSolid,
				    worldMaterial,
				    name + "_outer_container_lv");

  magnetContainerLV = new G4LogicalVolume(magnetContainerSolid,
					  worldMaterial,
					  name + "_container_lv");
  
  // VISUAL ATTRIBUTES
  G4VisAttributes* outerVisAttr;
  if (colour)
    {outerVisAttr = new G4VisAttributes(*colour);}
  else
    {outerVisAttr = new G4VisAttributes(*BDSColours::Instance()->GetColour("default"));}
  outerVisAttr->SetVisibility(true);
  outerVisAttr->SetForceLineSegmentsPerCircle(nSegmentsPerCircle);
  allVisAttributes.push_back(outerVisAttr);
  if (poleLV)
    {poleLV->SetVisAttributes(outerVisAttr);}
  yokeLV->SetVisAttributes(outerVisAttr);
  // container
  containerLV->SetVisAttributes(containerVisAttr);
  magnetContainerLV->SetVisAttributes(containerVisAttr);
}

void BDSMagnetOuterFactoryBase::SetUserLimits()
{
  if (poleLV)
    {poleLV->SetUserLimits(defaultUserLimits);}
  yokeLV->SetUserLimits(defaultUserLimits);
  containerLV->SetUserLimits(defaultUserLimits);
  magnetContainerLV->SetUserLimits(defaultUserLimits);

  for (auto& lv : allLogicalVolumes)
    {lv->SetUserLimits(defaultUserLimits);}
}

void BDSMagnetOuterFactoryBase::BuildMagnetContainerSolidAngled(G4String      name,
								G4double      magnetContainerLength,
								G4double      magnetContainerRadius)
{
  magnetContainerRadius += lengthSafetyLarge; // extra margin
  magnetContainerSolid = new G4CutTubs(name + "_container_solid",   // name
				       0,                           // inner radius
				       magnetContainerRadius,       // outer radius
				       magnetContainerLength * 0.5, // z half length
				       0,                           // starting angle
				       CLHEP::twopi,                // sweep angle
				       inputFaceNormal,             // input face normal vector
				       outputFaceNormal);           // output fae normal vector

  magContExtent = BDSExtent(magnetContainerRadius, magnetContainerRadius, magnetContainerLength*0.5);
}

void BDSMagnetOuterFactoryBase::BuildMagnetContainerSolidStraight(G4String name,
								  G4double magnetContainerLength,
								  G4double magnetContainerRadius)
{
  magnetContainerRadius += lengthSafetyLarge; // extra margin
  magnetContainerSolid = new G4Tubs(name + "_container_solid",   // name
				    0,                           // inner radius
				    magnetContainerRadius,       // outer radius
				    magnetContainerLength * 0.5, // z half length
				    0,                           // starting angle
				    CLHEP::twopi);               // sweep angle

  magContExtent = BDSExtent(magnetContainerRadius, magnetContainerRadius, magnetContainerLength*0.5);
}
  
void BDSMagnetOuterFactoryBase::CreateMagnetContainerComponent()
{
  magnetContainer = new BDSGeometryComponent(magnetContainerSolid,
					     magnetContainerLV,
					     magContExtent);
}


void BDSMagnetOuterFactoryBase::SetFaceNormals(BDSMagnetOuter* outer)
{
  outer->SetInputFaceNormal(inputFaceNormal);
  outer->SetOutputFaceNormal(outputFaceNormal);
}
  
