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
#ifndef BDSMATERIALS_H
#define BDSMATERIALS_H

#include <list>
#include <map>
#include <vector>

#include "globals.hh"
#include "G4Material.hh" // for G4State

class G4Element;
class G4MaterialPropertiesTable;

/**
 * @brief A class for all material definitions known to BDSIM.
 * Additional materials can be added in the parser as well.
 */

class BDSMaterials
{
public:

  /// Singleton pattern access
  static BDSMaterials* Instance();
  ~BDSMaterials(); 

  /// converts parser material list
  void PrepareRequiredMaterials(G4bool verbose = false);

  /// output available materials
  void ListMaterials()const;

  /// Get material by name
  G4Material* GetMaterial(G4String material) const;
  /// Get element by name
  G4Element*  GetElement(const G4String& symbol) const;

  /// Introduce materials loaded from GDML into this instance. If a prepend was used
  /// to load the material then it's cached both with and without it and the once without
  /// the prepend is flagged as a possible dupliate. If it's defined more than one without
  /// the prepend, then an exception will be thrown as the search is ambiguous.
  void CacheMaterialsFromGDML(const std::map<G4String, G4Material*>& materialsGDML,
                              const G4String& prepend,
                              G4bool prependWasUsed);

protected:
  BDSMaterials();
  /// Add G4Material
  void AddMaterial(G4Material* material, G4String name);
  /// Add alias to a material
  void AddExistingMaterialAlias(const G4String &existingMaterialName,
                                G4String alias);

  /** Add materials
      @param[in] Z        atomic number
      @param[in] A        mole mass in g/mole
      @param[in] density  in g/cm3
      @param[in] state    solid/gas
      @param[in] temp     in kelvin
      @param[in] pressure in atm
  */
  void AddMaterial(G4String name,
		   G4double Z,
		   G4double A,
		   G4double density,
		   G4State  state, 
		   G4double temperature, 
		   G4double pressure);

  /** Add materials
      @param[in] density    in g/cm3
      @param[in] state      solid/gas
      @param[in] temp       in kelvin
      @param[in] pressure   in atm
      @param[in] components list of elements
      @param[in] componentsFractions list of fractions or integers of the elements
  */
  template <typename Type>
  void AddMaterial(G4String name, 
		   G4double density, 
		   G4State  state, 
		   G4double temperature, 
		   G4double pressure,
		   const std::list<G4String>& components,
		   const std::list<Type>&     componentFractions);

  /// Return element if defined (nullptr if not)
  G4Element* CheckElement(const G4String& symbol) const;

private:
  /// Singleton instance
  static BDSMaterials* instance;

  ///@{ Methods called by constructor
  void DefineMetals();
  void DefineSuperconductors();
  void DefineNonMetalSolids();
  void DefineScintillators();
  void DefineLHCComponents();
  void DefineLiquids();
  void DefineGases();
  void DefinePlasmas();
  void DefineVacuums();
  ///@}

  ///@{ Add a G4Element
  void AddElement(G4Element* element, const G4String& symbol);
  void AddElement(const G4String& name, const G4String& symbol, G4double itsZ, G4double itsA);
  ///@}

  /// Print warning if density suspiciously high. Should be in g/cm3 in G4 units already.
  void DensityCheck(G4double  density,
		    const G4String& materialName) const;

  /// Print mass fractions of consituents of a given material.
  void PrintBasicMaterialMassFraction(G4Material* material) const;

  /// <ap of materials, convention name lowercase.
  std::map<G4String, G4Material*> materials;
  /// Maps of other names to existing materials. To avoid double deletion. Also in lower case.
  std::map<G4String, G4Material*> aliases;
  /// Map of names of loaded materials externally to the number of times loaded for possible
  /// duplicates.
  std::map<G4String, G4int> possibleDuplicates;
  /// Map of elements, no lowercase convention.
  std::map<G4String,G4Element*>  elements;
  /// Material tables for storing pointers
  std::vector<G4MaterialPropertiesTable*> propertiesTables;
  /// Create new properties table and store in vector
  G4MaterialPropertiesTable* CreatePropertiesTable();
};

#endif
