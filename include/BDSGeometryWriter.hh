/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2024.

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
#ifndef BDSGEOMETRYWRITER_H
#define BDSGEOMETRYWRITER_H

#include "globals.hh" // geant4 types / globals

class G4VPhysicalVolume;

/**
 * @brief A class for writing fully constructed geometry from BDSIM
 * out to other formats.
 * 
 * @author Laurie Nevay
 */

class BDSGeometryWriter
{
public:
  BDSGeometryWriter();
  ~BDSGeometryWriter();

  /// Dispatch function that matchest the geometry type and uses the appropraite
  /// member function to write out the geometry
  void ExportGeometry(G4String geometryType,
		      G4String geometryFileName);

private:

#ifdef USE_GDML
  /// Write the geometry to specified output filename. The filename should
  /// be the filename ending in .gdml. If the supplied volume is nullptr (default),
  /// the world volume will be written out.
  void WriteToGDML(G4String outputFileName, G4VPhysicalVolume* volume = nullptr);
#endif
};

#endif
