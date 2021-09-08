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
#ifndef COOLINGCHANNEL_H
#define COOLINGCHANNEL_H

#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include "published.h"

namespace GMAD
{
  /**
   * @brief Cooling channel parameters
   * 
   * @author Laurie Nevay
   */
  
  class CoolingChannel: public Published<CoolingChannel>
  {
  public:
    std::string name; ///< Object name

    int nCoils;
    std::list<double> coilInnerRadius;
    std::list<double> coilRadialThickness;
    std::list<double> coilLengthZ;
    std::list<double> coilCurrentDensity;
    std::list<double> coilOffsetZ;
    bool mirrorCoils;

    std::string absorberType;
    double      absorberCylinderLength;
    double      absorberCylinderRadius;
    double      absorberWedgeOpeningAngle;
    double      absorberWedgeRotationAngle;
    double      absorberWedgeDX;
    double      absorberWedgeDY;
    double      absorberWedgeApexToBase;

    int nRFCavities;
    std::list<double> rfOffsetZ;
    std::list<double> rfLength;
    std::list<double> rfVoltage;
    std::list<double> rfPhase;
    std::list<double> rfFrequency;
    std::list<double> rfWindowThickness;
    std::list<std::string> rfWindowMaterial;

    std::string magneticFieldModel;
    
    /// Constructor
    CoolingChannel();
    /// Reset
    void clear();
    /// Print some properties
    void print()const;
    /// Set methods by property name and value
    template <typename T>
    void set_value(const std::string& property, T value);
    // Template overloading for Array pointers
    /// Set method for lists
    void set_value(const std::string& property, Array* value);

  private:
    /// publish members
    void PublishMembers();
    
    std::map<std::string, std::list<double>*> attribute_map_list_double;
    std::map<std::string, std::list<std::string>*> attribute_map_list_string;
  };
  
  template <typename T>
  void CoolingChannel::set_value(const std::string& property, T value)
    {
#ifdef BDSDEBUG
      std::cout << "coolingchannel> Setting value " << std::setw(25) << std::left << property << value << std::endl;
#endif
      // member method can throw runtime_error, catch and exit gracefully
      try
	{set(this,property,value);}
      catch(const std::runtime_error&)
	{
	  std::cerr << "Error: coolingchannel> unknown option \"" << property << "\" with value " << value  << std::endl;
	  exit(1);
	}
    }
}

#endif
