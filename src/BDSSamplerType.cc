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
#include "BDSSamplerType.hh"

#include "globals.hh" // geant4 types / globals

#include <map>
#include <string>

// dictionary for BDSSamplerType
template<>
std::map<BDSSamplerType, std::string>* BDSSamplerType::dictionary =
  new std::map<BDSSamplerType, std::string> ({
  {BDSSamplerType::none,     "none"},
  {BDSSamplerType::plane,    "plane"},
  {BDSSamplerType::cylinder, "cylinder"}
});

BDSSamplerType BDS::DetermineSamplerType(G4String samplerType)
{
  std::map<G4String, BDSSamplerType> types;
  types["none"]     = BDSSamplerType::none;
  types["plane"]    = BDSSamplerType::plane;
  types["cylinder"] = BDSSamplerType::cylinder;

  samplerType.toLower();

  auto result = types.find(samplerType);
  if (result == types.end())
    {// it's not a valid key
      G4String msg = "\"" + samplerType + "\" is not a valid sampler type\n";
      for (const auto& it : types)
	{msg += "\"" + it.first + "\"\n";}
      throw BDSException(__METHOD_NAME__, msg);
    }
  
#ifdef BDSDEBUG
  G4cout << __METHOD_NAME__ << "determined sampler type to be " << result->second << G4endl;
#endif
  return result->second;
}
