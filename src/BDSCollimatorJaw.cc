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
#include "BDSCollimatorJaw.hh"
#include "BDSBeamPipeInfo.hh"
#include "BDSColours.hh"
#include "BDSDebug.hh"
#include "BDSException.hh"
#include "BDSMaterials.hh"
#include "BDSSDType.hh"
#include "BDSUtilities.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include <cmath>
#include <map>
#include <set>

BDSCollimatorJaw::BDSCollimatorJaw(G4String    nameIn,
				   G4double    lengthIn,
				   G4double    horizontalWidthIn,
				   G4double    xHalfGapIn,
				   G4double    yHalfHeightIn,
				   G4double    xSizeLeftIn,
				   G4double    xSizeRightIn,
				   G4bool      buildLeftJawIn,
				   G4bool      buildRightJawIn,
				   G4Material* collimatorMaterialIn,
				   G4Material* vacuumMaterialIn,
				   G4Colour*   colourIn):
  BDSCollimator(nameIn,
		lengthIn,
		horizontalWidthIn,
		"jcol",
		collimatorMaterialIn,
		vacuumMaterialIn,
		xHalfGapIn,
		yHalfHeightIn,
		xHalfGapIn,
		yHalfHeightIn,
		colourIn),
  jawSolid(nullptr),
  xSizeLeft(xSizeLeftIn),
  xSizeRight(xSizeRightIn),
  xHalfGap(xHalfGapIn),
  jawHalfWidth(0),
  yHalfHeight(yHalfHeightIn),
  buildLeftJaw(buildLeftJawIn),
  buildRightJaw(buildRightJawIn),
  buildAperture(true)
{
  jawHalfWidth = 0.5 * (0.5*horizontalWidth - lengthSafetyLarge - xHalfGap);
}

BDSCollimatorJaw::~BDSCollimatorJaw()
{;}

void BDSCollimatorJaw::CheckParameters()
{
  // BDSCollimator::CheckParameters() <- we replace this and don't call it - 'tapered' is never set
  if (!colour)
    {colour = BDSColours::Instance()->GetColour("collimator");}
  
  if (jawHalfWidth < 1e-3) // 1um minimum, could also be negative
    {throw BDSException(__METHOD_NAME__, "horizontalWidth insufficient given xsize of jcol \"" + name + "\"");}

  // set half height to half horizontal width if zero - finite height required.
  if (!BDS::IsFinite(yHalfHeight))
    {yHalfHeight = 0.5*horizontalWidth;}

  if (BDS::IsFinite(yHalfHeight) && (yHalfHeight < 1e-3)) // 1um minimum
    {throw BDSException(__METHOD_NAME__, "insufficient ysize for jcol \"" + name + "\"");}

  if ((yHalfHeight < 0) || ((yHalfHeight > 0) && (yHalfHeight < 1e-3))) // 1um minimum and not negative
    {throw BDSException(__METHOD_NAME__, "insufficient ysize for jcol \"" + name + "\"");}

  if (xSizeLeft < 0)
    {throw BDSException(__METHOD_NAME__, "left jcol jaw cannot have negative half aperture size: \"" + name + "\"");}
  if (xSizeRight < 0)
    {throw BDSException(__METHOD_NAME__, "left jcol jaw cannot have negative half aperture size: \"" + name + "\"");}

  if (std::abs(xSizeLeft) > 0.5*horizontalWidth)
    {
      G4cerr << __METHOD_NAME__ << "jcol \"" << name
	     << "\" left jaw offset is greater the element half width, jaw "
	     << "will not be constructed" << G4endl;
      buildLeftJaw = false;
    }
  if (std::abs(xSizeRight) > 0.5*horizontalWidth)
    {
      G4cerr << __METHOD_NAME__ << "jcol \"" << name
	     << "\" right jaw offset is greater the element half width, jaw "
	     << "will not be constructed" << G4endl;
      buildRightJaw = false;
    }
  
  if (!buildLeftJaw && !buildRightJaw)
    {throw BDSException(__METHOD_NAME__, "no jaws being built: \"" + name + "\"");}
  
  if (!BDS::IsFinite(xHalfGap) && !BDS::IsFinite(xSizeLeft) && !BDS::IsFinite(xSizeRight))
    {buildAperture = false;}
}

void BDSCollimatorJaw::BuildContainerLogicalVolume()
{
  containerSolid = new G4Box(name + "_container_solid",
			     horizontalWidth*0.5,
			     yHalfHeight,
			     chordLength*0.5);
  
  containerLogicalVolume = new G4LogicalVolume(containerSolid,
					       vacuumMaterial,
					       name + "_container_lv");
  BDSExtent ext(horizontalWidth * 0.5, yHalfHeight, chordLength*0.5);
  SetExtent(ext);
}

void BDSCollimatorJaw::Build()
{
  CheckParameters();
  BDSAcceleratorComponent::Build(); // calls BuildContainer and sets limits and vis for container

  // set each jaws half gap default to aperture half size
  G4double leftJawHalfGap = xHalfGap;
  G4double rightJawHalfGap = xHalfGap;

  // update jaw half gap with offsets
  if (BDS::IsFinite(xSizeLeft))
    {leftJawHalfGap = xSizeLeft;}
  if (BDS::IsFinite(xSizeRight))
    {rightJawHalfGap = xSizeRight;}
  
  // jaws have to fit inside containerLogicalVolume so calculate full jaw widths given offsets
  G4double leftJawWidth = 0.5 * horizontalWidth - leftJawHalfGap;
  G4double rightJawWidth = 0.5 * horizontalWidth - rightJawHalfGap;
  G4double vacuumWidth = 0.5 * (leftJawHalfGap + rightJawHalfGap);

  // centre of jaw and vacuum volumes for placements
  G4double leftJawCentre = 0.5*leftJawWidth + leftJawHalfGap;
  G4double rightJawCentre = 0.5*rightJawWidth + rightJawHalfGap;
  G4double vacuumCentre = 0.5*(leftJawHalfGap - rightJawHalfGap);

  G4ThreeVector leftJawPos = G4ThreeVector(leftJawCentre, 0, 0);
  G4ThreeVector rightJawPos = G4ThreeVector(-rightJawCentre, 0, 0);
  G4ThreeVector vacuumOffset = G4ThreeVector(vacuumCentre, 0, 0);

  G4VisAttributes* collimatorVisAttr = new G4VisAttributes(*colour);
  RegisterVisAttributes(collimatorVisAttr);

  // get appropriate user limits for jaw material
  G4UserLimits* collUserLimits = CollimatorUserLimits();

  // build jaws as appropriate
  if (buildLeftJaw && buildAperture)
    {
      G4VSolid* leftJawSolid = new G4Box(name + "_leftjaw_solid",
					 leftJawWidth * 0.5 - lengthSafety,
					 yHalfHeight - lengthSafety,
					 chordLength * 0.5 - lengthSafety);
      RegisterSolid(leftJawSolid);
      
      G4LogicalVolume* leftJawLV = new G4LogicalVolume(leftJawSolid,       // solid
						       collimatorMaterial,    // material
						       name + "_leftjaw_lv"); // name
      leftJawLV->SetVisAttributes(collimatorVisAttr);
      
      // user limits - provided by BDSAcceleratorComponent
      leftJawLV->SetUserLimits(collUserLimits);
      
      // register with base class (BDSGeometryComponent)
      RegisterLogicalVolume(leftJawLV);
      // register it in a set of collimator logical volumes
      BDSAcceleratorModel::Instance()->VolumeSet("collimators")->insert(leftJawLV);
      if (sensitiveOuter)
	{RegisterSensitiveVolume(leftJawLV, BDSSDType::collimatorcomplete);}
      
      // place the jaw
      G4PVPlacement* leftJawPV = new G4PVPlacement(nullptr,              // rotation
						   leftJawPos,              // position
						   leftJawLV,               // its logical volume
						   name + "_leftjaw_pv",    // its name
						   containerLogicalVolume,  // its mother volume
						   false,		             // no boolean operation
						   0,		                 // copy number
						   checkOverlaps);
      RegisterPhysicalVolume(leftJawPV);
    }
  if (buildRightJaw && buildAperture)
    {
      G4VSolid* rightJawSolid = new G4Box(name + "_rightjaw_solid",
					  rightJawWidth * 0.5 - lengthSafety,
					  yHalfHeight - lengthSafety,
					  chordLength * 0.5 - lengthSafety);
      RegisterSolid(rightJawSolid);
      
      G4LogicalVolume* rightJawLV = new G4LogicalVolume(rightJawSolid,      // solid
							collimatorMaterial,     // material
							name + "_rightjaw_lv"); // name
      rightJawLV->SetVisAttributes(collimatorVisAttr);
      
      // user limits - provided by BDSAcceleratorComponent
      rightJawLV->SetUserLimits(collUserLimits);
      
      // register with base class (BDSGeometryComponent)
      RegisterLogicalVolume(rightJawLV);
      // register it in a set of collimator logical volumes
      BDSAcceleratorModel::Instance()->VolumeSet("collimators")->insert(rightJawLV);
      if (sensitiveOuter)
	{RegisterSensitiveVolume(rightJawLV, BDSSDType::collimatorcomplete);}
      
      // place the jaw
      G4PVPlacement* rightJawPV = new G4PVPlacement(nullptr,             // rotation
						    rightJawPos,             // position
						    rightJawLV,              // its logical volume
						    name + "_rightjaw_pv",   // its name
						    containerLogicalVolume,  // its mother volume
						    false,		             // no boolean operation
						    0,		                 // copy number
						    checkOverlaps);
      RegisterPhysicalVolume(rightJawPV);
    }
  // if no aperture but the code has got to this stage, build the collimator as a simple box.
  if (!buildAperture)
    {
      collimatorSolid = new G4Box(name + "_solid",
				  horizontalWidth * 0.5 - lengthSafety,
				  yHalfHeight - lengthSafety,
				  chordLength * 0.5 - lengthSafety);
      RegisterSolid(collimatorSolid);
      
      G4LogicalVolume* collimatorLV = new G4LogicalVolume(collimatorSolid,       // solid
							  collimatorMaterial,    // material
							  name + "_lv");         // name
      collimatorLV->SetVisAttributes(collimatorVisAttr);
      
      // user limits - provided by BDSAcceleratorComponent - don't use collUserLimits
      collimatorLV->SetUserLimits(userLimits);
      
      // register with base class (BDSGeometryComponent)
      RegisterLogicalVolume(collimatorLV);
      if (sensitiveOuter)
	{RegisterSensitiveVolume(collimatorLV, BDSSDType::collimatorcomplete);}
      
      // place the jaw
      G4PVPlacement* collimatorPV = new G4PVPlacement(nullptr,                 // rotation
						      (G4ThreeVector) 0,       // position
						      collimatorLV,            // its logical volume
						      name + "_pv",    		   // its name
						      containerLogicalVolume,  // its mother volume
						      false,		           // no boolean operation
						      0,		               // copy number
						      checkOverlaps);
      RegisterPhysicalVolume(collimatorPV);
    }
  
  // build and place the vacuum volume only if the aperture is finite.
  if (buildAperture)
    {
      vacuumSolid = new G4Box(name + "_vacuum_solid",               // name
			      vacuumWidth - lengthSafety,           // x half width
			      yHalfHeight - lengthSafety,           // y half width
			      chordLength * 0.5);                   // z half length
      
      RegisterSolid(vacuumSolid);
      
      G4LogicalVolume* vacuumLV = new G4LogicalVolume(vacuumSolid,          // solid
						      vacuumMaterial,       // material
						      name + "_vacuum_lv"); // name
      
      vacuumLV->SetVisAttributes(containerVisAttr);
      // user limits - provided by BDSAcceleratorComponent
      vacuumLV->SetUserLimits(userLimits);
      SetAcceleratorVacuumLogicalVolume(vacuumLV);
      RegisterLogicalVolume(vacuumLV);
      if (sensitiveVacuum)
	{RegisterSensitiveVolume(vacuumLV, BDSSDType::energydepvacuum);}
      
      G4PVPlacement* vacPV = new G4PVPlacement(nullptr,                 // rotation
					       vacuumOffset,            // position
					       vacuumLV,                // its logical volume
					       name + "_vacuum_pv",     // its name
					       containerLogicalVolume,  // its mother  volume
					       false,                   // no boolean operation
					       0,                       // copy number
					       checkOverlaps);
      RegisterPhysicalVolume(vacPV);
    }
}
