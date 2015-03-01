/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 12.12.2004
   Copyright (c) 2004 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

#include "BDSBeamPipeType.hh"
#include "BDSBeamPipeFactory.hh"
#include "BDSGlobalConstants.hh" 

#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"               
#include "G4UserLimits.hh"
#include "G4Tubs.hh"

#include "globals.hh"
#include "BDSBeamPipeInfo.hh"
#include "BDSDebug.hh"
#include "BDSMultipole.hh"
#include "BDSMuSpoiler.hh"
#include "BDSMuSpoilerMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4FieldManager.hh"
#include "G4ClassicalRK4.hh"


/*
BDSMuSpoiler::BDSMuSpoiler (G4String& aName,G4double aLength,G4double bpRad,
			    G4double rInner, G4double rOuter,G4double aBField, 
                            std::list<G4double> blmLocZ, std::list<G4double> blmLocTheta,
                            G4String aTunnelMaterial):
  BDSAcceleratorComponent(aName,
			  aLength,bpRad,bpRad,bpRad,
                          blmLocZ, blmLocTheta, aTunnelMaterial),
  itsPhysiComp(NULL),itsPhysiComp2(NULL),itsPhysiInnerBP(NULL),
  itsPhysiBP(NULL),itsSolidLogVol(NULL),itsInnerLogVol(NULL),itsBeampipeLogicalVolume(NULL),
  itsInnerBPLogicalVolume(NULL),itsBPTube(NULL),itsInnerBPTube(NULL),
  itsBPVisAttributes(NULL),
  itsBeampipeRadius(bpRad),
  itsInnerRadius(rInner),
  itsOuterRadius(rOuter),
  itsBField(aBField),
  itsMagField(NULL),itsFieldMgr(NULL)
{
  G4double minInnRad =  itsBeampipeRadius + BDSGlobalConstants::Instance()->GetBeampipeThickness() + BDSGlobalConstants::Instance()->GetLengthSafety()/2.0;
  if( itsInnerRadius < minInnRad ){
    G4cout << "BDSMuSpoiler - WARNING - inner radius less than minimum. Changing inner radius from " << itsInnerRadius << " to " << minInnRad << G4endl;
    itsInnerRadius = minInnRad;
  }

  SetBPVisAttributes();
}
*/
BDSMuSpoiler::BDSMuSpoiler(G4String     name,
			   G4double     length,
			   G4double     bField,
			   beamPipeInfo beamPipeInfoIn,
			   G4double     boxSize,
			   G4String     outerMaterial,
			   G4String     tunnelMaterial,
			   G4double     tunnelRadius,
			   G4double     tunnelOffsetX):
  BDSMultipole(name,length,beamPipeInfoIn,boxSize,outerMaterial,tunnelMaterial,tunnelRadius,tunnelOffsetX),
  itsBField(bField)
{;}

/*
void BDSMuSpoiler::Build()
{
  BuildBPFieldAndStepper();

  BDSAcceleratorComponent::Build();
  if(BDSGlobalConstants::Instance()->GetBuildTunnel()){
    BuildTunnel();
  }
  BuildBeampipe();
  BuildBLMs();
  BuildMuSpoiler();
}

void BDSMuSpoiler::BuildMarkerLogicalVolume() 
{
  G4double xLength, yLength;
  G4double totalTunnelRadius = BDSGlobalConstants::Instance()->GetTunnelRadius()+BDSGlobalConstants::Instance()->GetTunnelThickness()+BDSGlobalConstants::Instance()->GetTunnelSoilThickness()+std::max(BDSGlobalConstants::Instance()->GetTunnelOffsetX(),BDSGlobalConstants::Instance()->GetTunnelOffsetY());
  
  xLength = yLength = std::max(itsOuterRadius,BDSGlobalConstants::Instance()->GetComponentBoxSize()/2);
  xLength = yLength = std::max(xLength,totalTunnelRadius);
  
#ifdef BDSDEBUG 
  G4cout<<"marker volume : x/y="<<xLength/CLHEP::m<<
    " m, l= "<<  (itsLength+BDSGlobalConstants::Instance()->GetLengthSafety())/2/CLHEP::m <<" m"<<G4endl;
#endif
  
  itsMarkerLogicalVolume=new G4LogicalVolume
    (new G4Box( itsName+"_solid",
                xLength,
		yLength,
		(itsLength-BDSGlobalConstants::Instance()->GetLengthSafety())/2), //z half length 
     BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetEmptyMaterial()),
     itsName+"_log");

  // now protect the fields inside the marker volume by giving the
  // marker a null magnetic field (otherwise G4VPlacement can
  // over-ride the already-created fields, by calling 
  // G4LogicalVolume::AddDaughter, which calls 
  // pDaughterLogical->SetFieldManager(fFieldManager, true) - the
  // latter 'true' over-writes all the other fields
  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobalConstants::Instance()->GetZeroFieldManager(),false);
}
*/

void BDSMuSpoiler::BuildBPFieldAndStepper()
{
  // don't do anything and leave as null pointers
  // don't worry, they won't be attached if null - whew
  return;
}

void BDSMuSpoiler::BuildBeamPipe()
{
  #ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << G4endl;
#endif
  
  beampipe = BDSBeamPipeFactory::Instance()->CreateBeamPipe(beamPipeType,
							    itsName,
							    itsLength,
							    aper1,
							    aper2,
							    aper3,
							    aper4,
							    vacuumMaterial,
							    beamPipeThickness,
							    beamPipeMaterial);

  itsMarkerLogicalVolume->
    SetFieldManager(BDSGlobalConstants::Instance()->GetZeroFieldManager(),false);

  RegisterLogicalVolumes(beampipe->GetAllLogicalVolumes());

  // place beampipe
  itsPhysiComp = new G4PVPlacement(0,                         // rotation
				   (G4ThreeVector)0,          // at (0,0,0)
				   beampipe->GetContainerLogicalVolume(),  // its logical volume
				   itsName+"_bmp_phys",	      // its name
				   itsMarkerLogicalVolume,    // its mother  volume
				   false,                     // no boolean operation
				   0, BDSGlobalConstants::Instance()->GetCheckOverlaps());// copy number

  //update extents - remember we don't know here if the outer volume is built
  //so this can be overwritten but acts as a minimum
  SetExtentX(beampipe->GetExtentX());
  SetExtentY(beampipe->GetExtentY());
  SetExtentZ(beampipe->GetExtentZ());

  /*
  // build beampipe
  G4Material *bpMaterial = BDSMaterials::Instance()->GetMaterial( BDSGlobalConstants::Instance()->GetBeamPipeMaterialName() );

#ifdef BDSDEBUG 
  G4cout << "Outer pipe :"
         << " r= " << itsBpRadius/CLHEP::m << " m"
         << " l= " << itsLength/(2.)/CLHEP::m << " m"
         << G4endl;
#endif

  itsBPTube=new G4Tubs(itsName+"_bmp_solid",
		       itsBpRadius+BDSGlobalConstants::Instance()->GetLengthSafety()/2.0,
		       itsBpRadius+BDSGlobalConstants::Instance()->GetBeampipeThickness(),
		       itsLength/(2.),
		       0,CLHEP::twopi*CLHEP::radian);

#ifdef BDSDEBUG
  G4cout << "Inner pipe :"
         << " r= " << (itsBpRadius-BDSGlobalConstants::Instance()->GetBeampipeThickness() )/CLHEP::m
         << " m"
         << " l= " << itsLength/(2.)/CLHEP::m << " m"
         << G4endl;
#endif
  
  itsInnerBPTube=new G4Tubs(itsName+"_inner_bmp_solid",
				0.,
				itsBpRadius,
			    itsLength/2 - BDSGlobalConstants::Instance()->GetLengthSafety()/2.0,
				0,CLHEP::twopi*CLHEP::radian);

  itsBeampipeLogicalVolume=	
    new G4LogicalVolume(itsBPTube,
			bpMaterial,
			itsName+"_bmp_log");

  itsInnerBPLogicalVolume=	
    new G4LogicalVolume(itsInnerBPTube,
			BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetVacuumMaterial()),
			itsName+"_inner_bmp_log");

  itsBeampipeLogicalVolume->SetVisAttributes(itsBPVisAttributes);

  if(BDSGlobalConstants::Instance()->GetSensitiveBeamPipe()){
    AddSensitiveVolume(itsBeampipeLogicalVolume);
  }

  itsPhysiInnerBP = new G4PVPlacement(
		      (G4RotationMatrix*)0,     // no rotation
                      (G4ThreeVector)0,         // at (0,0,0)
		      itsInnerBPLogicalVolume,  // its logical volume
		      itsName+"_inner_bmp_phys",// its name
		      itsMarkerLogicalVolume,   // its mother  volume
		      false,		        // no boolean operation
		      0, BDSGlobalConstants::Instance()->GetCheckOverlaps());// copy number
  
  itsPhysiBP = new G4PVPlacement(
			  (G4RotationMatrix*)0,	     // no rotation
			  (G4ThreeVector)0,	     // at (0,0,0)
			  itsBeampipeLogicalVolume,  // its logical volume
			  itsName+"_bmp_phys",	     // its name
			  itsMarkerLogicalVolume,    // its mother  volume
			  false,		     // no boolean operation
			  0, BDSGlobalConstants::Instance()->GetCheckOverlaps());// copy number
  //For geometric biasing etc.
  SetMultiplePhysicalVolumes(itsPhysiInnerBP);
  SetMultiplePhysicalVolumes(itsPhysiBP);

#ifndef NOUSERLIMITS
  G4UserLimits* AbsUserLimits =
    new G4UserLimits(DBL_MAX,DBL_MAX,BDSGlobalConstants::Instance()->GetMaxTime(),
		     BDSGlobalConstants::Instance()->GetThresholdCutCharged());

  AbsUserLimits->SetMaxAllowedStep(itsLength);

  itsInnerBPLogicalVolume->SetUserLimits(AbsUserLimits);
  itsBeampipeLogicalVolume->
    SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,BDSGlobalConstants::Instance()->GetMaxTime(),
				       BDSGlobalConstants::Instance()->GetThresholdCutCharged()));
#endif
  */
}

void BDSMuSpoiler::BuildOuterLogicalVolume(bool /*outerMaterialIsVacuum*/)
{
  //whole point is the outerlogical volume so ignore the outerMaterialIsVacuum flag
  
  //void BDSMuSpoiler::BuildMuSpoiler()
  //{

  BDSMultipole::BuildOuterLogicalVolume(false);

  // prepare and attach field
  outerMagField = new BDSMuSpoilerMagField(itsBField);
  outerFieldMgr = new G4FieldManager(outerMagField);
  if(BDSGlobalConstants::Instance()->GetDeltaIntersection()>0)
    {outerFieldMgr->SetDeltaIntersection(BDSGlobalConstants::Instance()->GetDeltaIntersection());}
  if(BDSGlobalConstants::Instance()->GetMinimumEpsilonStep()>0)
    {outerFieldMgr->SetMinimumEpsilonStep(BDSGlobalConstants::Instance()->GetMinimumEpsilonStep());}
  if(BDSGlobalConstants::Instance()->GetMaximumEpsilonStep()>0)
    {outerFieldMgr->SetMaximumEpsilonStep(BDSGlobalConstants::Instance()->GetMaximumEpsilonStep());}
  //if(BDSGlobalConstants::Instance()->GetDeltaOneStep()>0)
  //  {itsOuterFieldMgr->SetDeltaOneStep(BDSGlobalConstants::Instance()->GetDeltaOneStep());}
  itsOuterLogicalVolume->SetFieldManager(itsOuterFieldMgr,false);

  /*  
  itsSolidLogVol=
    new G4LogicalVolume(new G4Tubs(itsName+"_solid",
				   itsInnerRadius+BDSGlobalConstants::Instance()->GetLengthSafety()/2.0,
				   itsOuterRadius-BDSGlobalConstants::Instance()->GetLengthSafety()/2.0,
				   itsLength/2-BDSGlobalConstants::Instance()->GetLengthSafety()/2.0,
				   0,CLHEP::twopi*CLHEP::radian),
			BDSMaterials::Instance()->GetMaterial("Iron"), 
			itsName+"_solid");

  itsInnerLogVol=
    new G4LogicalVolume(new G4Tubs(itsName+"_inner",
				   0.,
				   itsInnerRadius,
				   itsLength/2,
				   0,CLHEP::twopi*CLHEP::radian),
			BDSMaterials::Instance()->GetMaterial(BDSGlobalConstants::Instance()->GetVacuumMaterial()),
			itsName+"_inner");

#ifndef NOUSERLIMITS
  G4UserLimits* AbsUserLimits =
    new G4UserLimits(DBL_MAX,DBL_MAX,BDSGlobalConstants::Instance()->GetMaxTime(),
		     BDSGlobalConstants::Instance()->GetThresholdCutCharged());

  AbsUserLimits->SetMaxAllowedStep(itsLength);
  itsInnerLogVol->SetUserLimits(AbsUserLimits);

  itsSolidLogVol->
    SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,BDSGlobalConstants::Instance()->GetMaxTime(),
				       BDSGlobalConstants::Instance()->GetThresholdCutCharged()));
#endif

  itsSolidLogVol->SetVisAttributes(itsVisAttributes);

  if(itsFieldMgr)
    {
      itsSolidLogVol->SetFieldManager(itsFieldMgr,false);
    }
  
  itsPhysiComp2 = 
    new G4PVPlacement(
		      (G4RotationMatrix*)0,		   // no rotation
		      (G4ThreeVector)0,                   // its position
		      itsInnerLogVol,      // its logical volume
		      itsName+"_combined", // its name
		      itsSolidLogVol,      // its mother  volume
		      false,		   // no boolean operation
		      0, BDSGlobalConstants::Instance()->GetCheckOverlaps());		   // copy number 

  //For geometric biasing etc.
  SetMultiplePhysicalVolumes(itsPhysiComp2);

  if(BDSGlobalConstants::Instance()->GetSensitiveComponents()){
    AddSensitiveVolume(itsSolidLogVol);
  }


  itsPhysiComp = 
    new G4PVPlacement(
		      (G4RotationMatrix*)0,		     // no rotation
		      (G4ThreeVector)0,                     // its position
		      itsSolidLogVol,    // its logical volume
		      itsName+"_solid",	     // its name
		      itsMarkerLogicalVolume, // its mother  volume
		      false,		     // no boolean operation
		      0, BDSGlobalConstants::Instance()->GetCheckOverlaps());		     // copy number  

  //For geometric biasing etc.
  SetMultiplePhysicalVolumes(itsPhysiComp);
}
  */
  /*
void BDSMuSpoiler::BuildBLMs(){
  itsBlmLocationR=itsOuterRadius;
  BDSAcceleratorComponent::BuildBLMs();
  */
}


void BDSMuSpoiler::SetVisAttributes()
{
  itsVisAttributes=new G4VisAttributes(G4Colour(0.0,0.5,0.5));
  itsVisAttributes->SetForceSolid(true);
  itsVisAttributes->SetVisibility(true);
}

/*
void BDSMuSpoiler::SetBPVisAttributes()
{
  itsBPVisAttributes=new G4VisAttributes(G4Colour(0.2,0.2,0.2));
  itsBPVisAttributes->SetForceSolid(true);
  itsBPVisAttributes->SetVisibility(true);
}
*/

BDSMuSpoiler::~BDSMuSpoiler()
{
  delete outerMagField;
  delete outerFieldMgr;
  
  // delete itsBPVisAttributes;

  //   delete itsInnerTunnelUserLimits;
  //   delete itsTunnelUserLimits;
  //   delete itsSoilTunnelUserLimits;
  
  //   delete itsBPTube;
  //   delete itsInnerBPTube;
  
  //  delete itsPhysiComp;
  //  delete itsPhysiComp2;
  //  delete itsPhysiInnerBP;
  //  delete itsPhysiBP;
  //  delete itsPhysiCompSoil;

  //delete itsMagField;
  //delete itsFieldMgr;

  //   delete itsSoilTube;
  //   delete itsTunnelTube;
  //   delete itsInnerTunnelTube;
  //   delete itsInnerTunnelLogicalVolume;
  //   delete itsSoilTunnelLogicalVolume;
}
