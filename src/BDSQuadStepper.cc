/* BDSIM code.    Version 1.0
   Author: Grahame A. Blair, Royal Holloway, Univ. of London.
   Last modified 24.7.2002
   Copyright (c) 2002 by G.A.Blair.  ALL RIGHTS RESERVED. 
*/

// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: BDSQuadStepper.cc,v 1.5 2006/04/19 21:51:58 carter Exp $
// GEANT4 tag $Name:  $
//
#include "BDSQuadStepper.hh"
#include "G4ThreeVector.hh"
#include "G4LineSection.hh"
#include "G4TransportationManager.hh"
#include "G4Track.hh"
#include "G4UserLimits.hh"
using std::max;
extern G4double BDSLocalRadiusOfCurvature;
extern G4int event_number;

BDSQuadStepper::BDSQuadStepper(G4Mag_EqRhs *EqRhs)
  : G4MagIntegratorStepper(EqRhs,6)  // integrate over 6 variables only !!
                                       // position & velocity
{
  fPtrMagEqOfMot = EqRhs;
}


void BDSQuadStepper::AdvanceHelix( const G4double  yIn[],
				  G4ThreeVector Bfld,
				  G4double  h,
				  G4double  yQuad[])
{
  G4ThreeVector positionMove, endTangent;

  const G4double *pIn = yIn+3;
  G4ThreeVector v0= G4ThreeVector( pIn[0], pIn[1], pIn[2]);  

  G4ThreeVector GlobalPosition= G4ThreeVector( yIn[0], yIn[1], yIn[2]);  
  G4double InitMag=v0.mag();
  G4ThreeVector InitMomDir=v0.unit();

  G4double kappa= - fPtrMagEqOfMot->FCof()*itsBGrad/InitMag;
  //  G4cout<<" at start: p="<<-  0.299792458 /(tesla*m)/
  //					     fPtrMagEqOfMot->FCof()
  //					     <<G4endl;

  G4double h2=h*h;

  G4ThreeVector LocalR,LocalRp ;
  
 //  G4cout << "kappa: " << kappa << G4endl;
//   G4cout << "InitMag: " << InitMag << G4endl;
//   G4cout << "itsBGrad: " << itsBGrad << G4endl;
//   G4cout << "fPtrMagEqOfMot->FCof(): " << fPtrMagEqOfMot->FCof() << G4endl << G4endl;
  // relevant momentum scale is p_z, not P_tot:
  // check that the approximations are valid, else do a linear step:
  if(fabs(kappa)<1.e-12)
    {
      positionMove  = (h/InitMag) * v0;

      yQuad[0]   = yIn[0] + positionMove.x(); 
      yQuad[1]   = yIn[1] + positionMove.y(); 
      yQuad[2]   = yIn[2] + positionMove.z(); 
				
      yQuad[3] = v0.x();
      yQuad[4] = v0.y();
      yQuad[5] = v0.z();

      itsDist=0;
    }

  else 
    { 
      G4Navigator* QuadNavigator=
      	G4TransportationManager::GetTransportationManager()->
      	GetNavigatorForTracking();
      if(h > itsVolLength)
	{
	  // added steplength check for large h
	  // (causes NAN with cosh and sinh later)
	  h=itsVolLength;
	  h2=h*h;
	}
      G4AffineTransform LocalAffine=QuadNavigator-> 
      	GetLocalToGlobalTransform();

      G4AffineTransform GlobalAffine=QuadNavigator->
	GetGlobalToLocalTransform();
      G4ThreeVector LocalR=GlobalAffine.TransformPoint(GlobalPosition); 
      G4ThreeVector LocalRp=GlobalAffine.TransformAxis(InitMomDir);


      G4double x0,xp,y0,yp,z0,zp;
      G4double x1,x1p,y1,y1p,z1,z1p;
      x0=LocalR.x();
      y0=LocalR.y();
      z0=LocalR.z();
      xp=LocalRp.x();
      yp=LocalRp.y();
      zp=LocalRp.z();

       // local r'' (for curvature)
      G4ThreeVector LocalRpp;
      LocalRpp.setX(-zp*x0);
      LocalRpp.setY( zp*y0);
      LocalRpp.setZ( x0*xp - y0*yp);

      LocalRpp*=kappa;

       // determine effective curvature
      G4double R_1 = LocalRpp.mag();
      if(R_1>0.)
	{
	  G4double R=1./R_1;

	  // Save for Synchrotron Radiation calculations:
	  BDSLocalRadiusOfCurvature=R;

	  // chord distance (simple quadratic approx)
	  itsDist= h2/(8*R);

	  // check for paraxial approximation:
	  if((fabs(zp)>0.99)&&(fabs(kappa)<1.e-6))
	    {
	      G4double rootK=sqrt(fabs(kappa*zp));
	      G4double rootKh=rootK*h*zp;
	      G4double X11,X12,X21,X22;
	      G4double Y11,Y12,Y21,Y22;

	      if (kappa>0)
		{
		  X11= cos(rootKh);
		  X12= sin(rootKh)/rootK;
		  X21=-fabs(kappa)*X12;
		  X22= X11;
		  
		  Y11= cosh(rootKh);
		  Y12= sinh(rootKh)/rootK;
		  Y21= fabs(kappa)*Y12;
		  Y22= Y11;
		}
	      else //if (kappa<0)
		{
		  X11= cosh(rootKh);
		  X12= sinh(rootKh)/rootK;
		  X21= fabs(kappa)*X12;
		  X22= X11;
		  
		  Y11= cos(rootKh);
		  Y12= sin(rootKh)/rootK;
		  Y21= -fabs(kappa)*Y12;
		  Y22= Y11;
		}

	      x1      = X11*x0 + X12*xp;    
	      x1p= X21*x0 + X22*xp;
	      
	      y1      = Y11*y0 + Y12*yp;    
	      y1p= Y21*y0 + Y22*yp;
	      
	      z1p=sqrt(1 - x1p*x1p -y1p*y1p);

	      G4double dx=x1-x0;
	      G4double dy=y1-y0;
	      // Linear chord length
              G4double dR2=dx*dx+dy*dy;
	      G4double dz=sqrt(h2*(1.-h2/(12*R*R))-dR2);
	      
	   // check for precision problems
	      G4double ScaleFac=(dx*dx+dy*dy+dz*dz)/h2;
	      if(ScaleFac>1.0000001)
		{
		  ScaleFac=sqrt(ScaleFac);
		  dx/=ScaleFac;
		  dy/=ScaleFac;
		  dz/=ScaleFac;
		  x1=x0+dx;
		  y1=y0+dy;
		}
	      z1=z0+dz;
	    }
	  else
	    // perform local helical steps (paraxial approx not safe)
	    {
	      // simple quadratic approx:	      
	      G4double quadX= - kappa*x0*zp;
	      G4double quadY=   kappa*y0*zp;
	      G4double quadZ=   kappa*(x0*xp - y0*yp);
	      
	      // determine maximum curvature:
	      G4double maxCurv=max(fabs(quadX),fabs(quadY));
	      maxCurv=max(maxCurv,fabs(quadZ));
	      
	      x1=x0 + h*xp + quadX*h2/2;
	      y1=y0 + h*yp + quadY*h2/2; 
	      z1=z0 + h*zp   + quadZ*h2/2;
	      
	      x1p=xp + quadX*h;
	      y1p=yp + quadY*h;
	      z1p=zp + quadZ*h;
	      
	      // estimate parameters at end of step:
	      G4double quadX_end= - kappa*x1*z1p;
	      G4double quadY_end=   kappa*y1*z1p;
	      G4double quadZ_end=   kappa*(x1*x1p - y1*y1p);
	      
	      // determine maximum curvature:
	      maxCurv=max(maxCurv,fabs(quadX_end));
	      maxCurv=max(maxCurv,fabs(quadY_end));
	      maxCurv=max(maxCurv,fabs(quadZ_end));

	      itsDist=maxCurv*h2/4;
	      

	      // use the average:
	      G4double quadX_av=(quadX+quadX_end)/2;
	      G4double quadY_av=(quadY+quadY_end)/2;
	      G4double quadZ_av=(quadZ+quadZ_end)/2;
	      
	      G4double x_prime_av=(xp + x1p)/2;
	      G4double y_prime_av=(yp + y1p)/2;
	      G4double z_prime_av=(zp + z1p)/2;
	      
	      x1=x0 + h*x_prime_av + quadX_av * h2/2;
	      y1=y0 + h*y_prime_av + quadY_av * h2/2; 
	      z1=z0 + h*z_prime_av + quadZ_av * h2/2;
	      
	      x1p=xp + quadX_av*h;
	      y1p=yp + quadY_av*h;
	      z1p=zp + quadZ_av*h;
	      
	      G4double dx=(x1-x0);
	      G4double dy=(y1-y0);
	      G4double dz=(z1-z0);
	      G4double chord2=dx*dx + dy*dy + dz*dz;
	      if(chord2>h2)
		{G4double hnew=h*sqrt(h2/chord2);
		x1=x0 + hnew*x_prime_av + quadX_av * hnew*hnew/2;
		y1=y0 + hnew*y_prime_av + quadY_av * hnew*hnew/2; 
		z1=z0 + hnew*z_prime_av + quadZ_av * hnew*hnew/2;
		
		x1p=xp + quadX_av*hnew;
		y1p=yp + quadY_av*hnew;
		z1p=zp + quadZ_av*hnew;
		}
	    }

	  LocalR.setX(x1);
	  LocalR.setY(y1);
	  LocalR.setZ(z1);
	  
	  LocalRp.setX(x1p);
	  LocalRp.setY(y1p);
	  LocalRp.setZ(z1p);

	}
       else
	 {LocalR += h*LocalRp;
	 itsDist=0.;}

      G4ThreeVector GlobalTangent;
      GlobalPosition=LocalAffine.TransformPoint(LocalR); 
      GlobalTangent=LocalAffine.TransformAxis(LocalRp);

      GlobalTangent*=InitMag;

      yQuad[0]   = GlobalPosition.x(); 
      yQuad[1]   = GlobalPosition.y(); 
      yQuad[2]   = GlobalPosition.z(); 

				
      yQuad[3] = GlobalTangent.x();
      yQuad[4] = GlobalTangent.y();
      yQuad[5] = GlobalTangent.z();

    }
}    


void BDSQuadStepper::Stepper( const G4double yInput[],
			     const G4double dydx[],
			     const G4double hstep,
			     G4double yOut[],
			     G4double yErr[]      )
{  
  const G4int nvar = 6 ;
  G4int i;
  const G4double *pIn = yInput+3;
  G4ThreeVector v0= G4ThreeVector( pIn[0], pIn[1], pIn[2]);  
  G4double InitMag=v0.mag();
  G4double kappa= - fPtrMagEqOfMot->FCof()*itsBGrad/InitMag;
  
  if(fabs(kappa)<1.e-6) //kappa is small - no error needed for paraxial treatment
    {
      for(i=0;i<nvar;i++) yErr[i]=0;
      AdvanceHelix(yInput,0,hstep,yOut);
    }
  else   //need to compute errors for helical steps
    {
      G4double yTemp[7], yIn[7];
      
      //  Saving yInput because yInput and yOut can be aliases for same array
      
      for(i=0;i<nvar;i++) yIn[i]=yInput[i];
      
      G4double h = hstep * 0.5; 
      if(h>itsVolLength) h = itsVolLength/2.;
      // Do two half steps
      AdvanceHelix(yIn,   0,  h, yTemp);
      AdvanceHelix(yTemp, 0, h, yOut); 
      
      // Do a full Step
      h = hstep ;
      if(h>itsVolLength) h = itsVolLength;
      AdvanceHelix(yIn, 0, h, yTemp); 
      
      for(i=0;i<nvar;i++) yErr[i] = yOut[i] - yTemp[i] ;
    }
  return;
}

G4double BDSQuadStepper::DistChord()   const 
{

return itsDist;
  // This is a class method that gives distance of Mid 
  //  from the Chord between the Initial and Final points.
}

BDSQuadStepper::~BDSQuadStepper()
{}
