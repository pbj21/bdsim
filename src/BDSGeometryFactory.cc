/* 
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
University of London 2001 - 2021.

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
#include "BDSSDEnergyDeposition.hh"
#include "BDSGeometryExternal.hh"
#include "BDSGeometryFactory.hh"
#include "BDSGeometryFactoryBase.hh"
#ifdef USE_GDML
#include "BDSGeometryFactoryGDML.hh"
#endif
#include "BDSGeometryFactoryGMAD.hh"
#include "BDSGeometryFactorySQL.hh"
#include "BDSGeometryType.hh"
#include "BDSSDType.hh"
#include "BDSUtilities.hh"

#include "G4String.hh"
#include "G4Types.hh"

#include <string>
#include <unordered_map>
#include <utility>

BDSGeometryFactory* BDSGeometryFactory::instance = nullptr;

BDSGeometryFactory* BDSGeometryFactory::Instance()
{
  if (!instance)
    {instance = new BDSGeometryFactory();}
  return instance;
}

BDSGeometryFactory::BDSGeometryFactory()
{
#ifdef USE_GDML
  gdml = new BDSGeometryFactoryGDML();
#else
  gdml = nullptr;
#endif
  gmad = new BDSGeometryFactoryGMAD();
  sql  = new BDSGeometryFactorySQL();
}

BDSGeometryFactory::~BDSGeometryFactory()
{
  delete gdml;
  delete gmad;
  delete sql;
  for (auto& geom : storage)
    {delete geom;}
  instance = nullptr;
}

BDSGeometryFactoryBase* BDSGeometryFactory::GetAppropriateFactory(BDSGeometryType type)
{
  switch(type.underlying())
    {
#ifdef USE_GDML
    case BDSGeometryType::gdml:
      {return gdml; break;}
#endif
    case BDSGeometryType::gmad:
      {return gmad; break;}
    case BDSGeometryType::mokka:
      {return sql; break;}
    default:
      {
	G4cout << "Unsupported factory type " << type;
	return nullptr;
      }
    }
}

BDSGeometryExternal* BDSGeometryFactory::BuildGeometry(const G4String&  componentName,
						       const G4String&  formatAndFileName,
						       std::map<G4String, G4Colour*>* colourMapping,
						       G4bool                 autoColour,
						       G4double               suggestedLength,
						       G4double               suggestedHorizontalWidth,
						       std::vector<G4String>* namedVacuumVolumes,
						       G4bool                 makeSensitive,
						       BDSSDType              sensitivityType,
                                                       G4bool                 stripOuterVolumeAndMakeAssembly)
{
  std::pair<G4String, G4String> ff = BDS::SplitOnColon(formatAndFileName);
  G4String fileName = BDS::GetFullPath(ff.second);

  G4String searchName = fileName;
  // If we strip the outer volume we're technically modifying the geometry
  // and should prepare a unique load of it and cache that otherwise, we
  // will get either the original or the stripped version only if we place
  // the load the same geometry twice with/without stripping
  if (stripOuterVolumeAndMakeAssembly)
    {searchName += "_stripped";}
  const auto search = registry.find(searchName);
  if (search != registry.end())
    {return search->second;}// it was found already in registry
  // else wasn't found so continue

  // Check the file exists.
  if (!BDS::FileExists(fileName))
    {throw BDSException(__METHOD_NAME__, "No such file \"" + fileName + "\"");}
  
  BDSGeometryType format = BDS::DetermineGeometryType(ff.first);
  BDSGeometryFactoryBase* factory = GetAppropriateFactory(format);
  if (!factory)
    {return nullptr;}
  
  BDSGeometryExternal* result = factory->Build(componentName, fileName, colourMapping, autoColour,
					       suggestedLength, suggestedHorizontalWidth,
					       namedVacuumVolumes);
  
  if (result)
    {
      if (stripOuterVolumeAndMakeAssembly)
        {result->StripOuterAndMakeAssemblyVolume();}
      // Set all volumes to be sensitive.
      if (makeSensitive)
	{result->MakeAllVolumesSensitive(sensitivityType);}
      
      registry[(std::string)searchName] = result; // cache using optionally modified name
      storage.insert(result);
    }
  
  return result;
}
