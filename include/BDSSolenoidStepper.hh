//  
//   BDSIM, (C) 2001-2007
//   
//   version 0.4
//

//
// class BDSSolenoidStepper
//
// Class description:
// stepper for solenoid magnetic field

// History:
//

#ifndef BDSSOLENOIDSTEPPER_HH
#define BDSSOLENOIDSTEPPER_HH
#include "globals.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_EqRhs.hh"
#include "G4ThreeVector.hh"
#include "G4Navigator.hh"

class BDSSolenoidStepper : public G4MagIntegratorStepper
{

public:  // with description

  BDSSolenoidStepper(G4Mag_EqRhs *EqRhs);

  ~BDSSolenoidStepper();

  void Stepper( const G4double y[],
		const G4double dydx[],
		const G4double h,
		G4double yout[],
		G4double yerr[]  );
  // The stepper for the Runge Kutta integration.
  // The stepsize is fixed, equal to h.
  // Integrates ODE starting values y[0 to 6]
  // Outputs yout[] and its estimated error yerr[].

  G4double DistChord()   const;
  // Estimate maximum distance of curved solution and chord ... 
 
  void SetBField(G4double aBField);
  G4double GetBField();

  void StepperName();

public: // without description
  
  G4int IntegratorOrder()const { return 2; }

protected:
  //  --- Methods used to implement all the derived classes -----

  void AdvanceHelix( const G4double  yIn[],
		     G4ThreeVector Bfld,
		     G4double  h,
		     G4double  yOut[]);    // output 
  // A first order Step along a solenoid inside the field.

private:
  
  G4Mag_EqRhs*  fPtrMagEqOfMot;

  G4double itsBField;

  G4double itsDist;

};

inline  void BDSSolenoidStepper::SetBField(G4double aBField)
{itsBField=aBField;
}

inline G4double BDSSolenoidStepper::GetBField()
{return itsBField;}

inline void BDSSolenoidStepper::StepperName()
{G4cout<<"BDSSolenoidStepper"<<G4endl;}

#endif /* BDSSOLENOIDSTEPPER_HH */
