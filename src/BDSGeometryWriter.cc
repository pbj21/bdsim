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
#include "BDSDebug.hh"
#include "BDSException.hh"
#include "BDSGeometryWriter.hh"
#include "BDSUtilities.hh"

#include "globals.hh" // geant4 types / globals
#ifdef USE_GDML
#include "G4GDMLParser.hh"
#endif
#include "G4FileUtilities.hh"
#include "G4String.hh"
#include "G4TransportationManager.hh"

class G4VPhysicalVolume;

BDSGeometryWriter::~BDSGeometryWriter()
{;}

BDSGeometryWriter::BDSGeometryWriter()
{;}

void BDSGeometryWriter::ExportGeometry(G4String geometryType,
				       G4String geometryFileName)
{
  if (geometryType == "gdml")
    {
#ifdef USE_GDML
      WriteToGDML(geometryFileName);
#else
      throw BDSException(__METHOD_NAME__, "Unable to write out " + geometryFileName + ", as compiled without GDML support");
#endif
    }
}

#ifdef USE_GDML
void BDSGeometryWriter::WriteToGDML(G4String           outputFileName,
				    G4VPhysicalVolume* volume)
{
  if (!volume)
    {volume = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();}
  // gdml parser throws exception if file exists. Delete file in that case
  if (BDS::FileExists(outputFileName))
    {
      G4FileUtilities fileUtilities;
      G4cout << "Removing existing file: \"" << outputFileName << "\"" << G4endl;
      fileUtilities.DeleteFile(outputFileName, "");
    }

  // if the filename is in part a path to a directory, check that directory exists
  // otherwise the GDML writer will give an abort trap bad access
  if (BDS::StrContains(outputFileName, "/"))
    {
      G4String dirName = BDS::GetFullPath(outputFileName, true);
      if (!BDS::DirectoryExists(dirName))
        {throw BDSException(__METHOD_NAME__, "directory \"" + dirName + "\" does not exist - please create it first.");}
    }

  G4GDMLParser parser;
  parser.Write(outputFileName, volume, true);
}
#endif
