#ifndef BDSFIELDMAGDECAPOLE_H
#define BDSFIELDMAGDECAPOLE_H

#include "BDSField.hh"

#include "globals.hh" // geant4 types / globals
#include "G4ThreeVector.hh"

class BDSMagnetStrength;

/**
 * @brief Class that provides the magnetic strength in a decapole.
 * 
 * The magnetic field is calculated from the decapole strength parameter
 * "k4" and a design rigidity (brho).
 */

class BDSFieldMagDecapole: public BDSField
{
public:
  BDSFieldMagDecapole(const BDSMagnetStrength* strength,
		      const G4double           brho);
  
  virtual ~BDSFieldMagDecapole(){;}

  virtual G4ThreeVector GetFieldValue(const G4ThreeVector& position) const;
  
private:
  /// Private default constructor to force use of supplied constructor.
  BDSFieldMagDecapole();

  /// B'''' - the fourth derivative of the magnetic field.
  G4double bQuadruplePrime;

  /// B'''' / 4!
  G4double bQPNormed;
};

#endif
