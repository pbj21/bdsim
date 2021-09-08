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
#include "coolingchannel.h"

using namespace GMAD;

CoolingChannel::CoolingChannel()
{
  clear();
  PublishMembers();
}

void CoolingChannel::clear()
{
  name = "";
  nCoils = 0;
  coilInnerRadius.clear();
  coilRadialThickness.clear();
  coilLengthZ.clear();
  coilCurrentDensity.clear();
  coilOffsetZ.clear();
  coilMaterial.clear();
  mirrorCoils = false;
  absorberType = "";
  absorberCylinderLength = 0;
  absorberCylinderRadius = 0;
  absorberWedgeOpeningAngle = 0;
  absorberWedgeRotationAngle = 0;
  absorberWedgeDX = 0;
  absorberWedgeDY = 0;
  absorberWedgeApexToBase = 0;
  nRFCavities = 0;
  rfOffsetZ.clear();
  rfLength.clear();
  rfVoltage.clear();
  rfPhase.clear();
  rfFrequency.clear();
  rfWindowThickness.clear();
  rfWindowMaterial.clear();
  magneticFieldModel = "block";
}

void CoolingChannel::PublishMembers()
{
  publish("name",                 &CoolingChannel::name);

  publish("nCoils",               &CoolingChannel::nCoils);
  publish("coilInnerRadius",      &CoolingChannel::coilInnerRadius);
  publish("coilRadialThickness",  &CoolingChannel::coilRadialThickness);
  publish("coilLengthZ",          &CoolingChannel::coilLengthZ);
  publish("coilCurrentDensity",   &CoolingChannel::coilCurrentDensity);
  publish("coilOffsetZ",          &CoolingChannel::coilOffsetZ);
  publish("coilMaterial",         &CoolingChannel::coilMaterial);
  publish("mirrorCoils",          &CoolingChannel::mirrorCoils);

  publish("absorberType",               &CoolingChannel::absorberType);
  publish("absorberCylinderLength",     &CoolingChannel::absorberCylinderLength);
  publish("absorberCylinderRadius",     &CoolingChannel::absorberCylinderRadius);
  publish("absorberWedgeOpeningAngle",  &CoolingChannel::absorberWedgeOpeningAngle);
  publish("absorberWedgeRotationAngle", &CoolingChannel::absorberWedgeRotationAngle);
  publish("absorberWedgeDX",            &CoolingChannel::absorberWedgeDX);
  publish("absorberWedgeDY",            &CoolingChannel::absorberWedgeDY);
  publish("absorberWedgeApexToBase",    &CoolingChannel::absorberWedgeApexToBase);

  publish("nRFCavities",       &CoolingChannel::nRFCavities);
  publish("rfOffsetZ",         &CoolingChannel::rfOffsetZ);
  publish("rfLength",          &CoolingChannel::rfLength);
  publish("rfVoltage",         &CoolingChannel::rfVoltage);
  publish("rfPhase",           &CoolingChannel::rfPhase);
  publish("rfFrequency",       &CoolingChannel::rfFrequency);
  publish("rfWindowThickness", &CoolingChannel::rfWindowThickness);
  publish("rfWindowMaterial",  &CoolingChannel::rfWindowMaterial);

  publish("magneticFieldModel",&CoolingChannel::magneticFieldModel);
  
  attribute_map_list_double["coilInnerRadius"]     = &coilInnerRadius;
  attribute_map_list_double["coilRadialThickness"] = &coilRadialThickness;
  attribute_map_list_double["coilLengthZ"]         = &coilLengthZ;
  attribute_map_list_double["coilCurrentDensity"]  = &coilCurrentDensity;
  attribute_map_list_double["coilOffsetZ"]         = &coilOffsetZ;
  attribute_map_list_string["coilMaterial"]        = &coilMaterial;
  attribute_map_list_double["rfOffsetZ"]           = &rfOffsetZ;
  attribute_map_list_double["rfLength"]            = &rfLength;
  attribute_map_list_double["rfVoltage"]           = &rfVoltage;
  attribute_map_list_double["rfPhase"]             = &rfPhase;
  attribute_map_list_double["rfFrequency"]         = &rfFrequency;
  attribute_map_list_double["rfWindowThickness"]   = &rfWindowThickness;
  attribute_map_list_string["rfWindowMaterial"]    = &rfWindowMaterial;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& l);

void CoolingChannel::print()const
{
  std::cout << "coolingchannel: "
	    << "name "                       << name                       << std::endl
	    << "nCoils "                     << nCoils                     << std::endl
	    << "coilInnerRadius "            << coilInnerRadius            << std::endl
	    << "coilRadialThickness "        << coilRadialThickness        << std::endl
	    << "coilLengthZ "                << coilLengthZ                << std::endl
	    << "coilCurrentDensity "         << coilCurrentDensity         << std::endl
	    << "coilOffsetZ "                << coilOffsetZ                << std::endl
	    << "coilMaterial "               << coilMaterial               << std::endl
	    << "mirrorCoils "                << mirrorCoils                << std::endl
	    << "absorberType "               << absorberType               << std::endl
	    << "absorberCylinderLength "     << absorberCylinderLength     << std::endl
	    << "absorberCylinderRadius "     << absorberCylinderRadius     << std::endl
	    << "absorberWedgeOpeningAngle "  << absorberWedgeOpeningAngle  << std::endl
	    << "absorberWedgeRotationAngle " << absorberWedgeRotationAngle << std::endl
	    << "absorberWedgeDX "            << absorberWedgeDX            << std::endl
    	    << "absorberWedgeDY "            << absorberWedgeDY            << std::endl
	    << "absorberWedgeApexToBase "    << absorberWedgeApexToBase    << std::endl
	    << "nRFCavities "                << nRFCavities                << std::endl
	    << "rfOffsetZ "                  << rfOffsetZ                  << std::endl
	    << "rfLength"                    << rfLength                   << std::endl
	    << "rfVoltage "                  << rfVoltage                  << std::endl
	    << "rfPhase "                    << rfPhase                    << std::endl
	    << "rfFrequency "                << rfFrequency                << std::endl
	    << "rfWindowThickness "          << rfWindowThickness          << std::endl
	    << "rfWindowMaterial "           << rfWindowMaterial           << std::endl
	    << "magneticFieldModel "         << magneticFieldModel         << std::endl;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& l)
{
  out << "{";
  for (const auto& v : l)
    {out << v << ", ";}
  out << "}";
  return out;
}

void CoolingChannel::set_value(const std::string& property, Array* value)
{
  auto search = attribute_map_list_double.find(property);
  if (search != attribute_map_list_double.end())
    {value->set_vector(*search->second);}
  else
    {
      auto search2 = attribute_map_list_string.find(property);
      if (search2 != attribute_map_list_string.end())
	{value->set_vector(*search2->second);}
      else
	{
	  std::cerr << "Error: parser> unknown coolingchannel option \"" << property << "\", or doesn't expect vector type" << std::endl;
	  exit(1);
	}
    }
}
