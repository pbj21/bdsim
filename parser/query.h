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
#ifndef QUERY_H
#define QUERY_H

#include <iomanip>
#include <iostream>
#include <string>

#include "published.h"

namespace GMAD
{
  /**
   * @brief Query structure class for parser.
   * 
   * @author Laurie Nevay
   */
  
  class Query: public Published<Query>
  {
  public:
    std::string name;   ///< Name of object.
    int nDimensions;    ///< Number of dimensions to query.
    int nx;             ///< Number of samples in x.
    int ny;             ///< Number of samples in y.
    int nz;             ///< Number of samples in z.
    int nt;             ///< Number of samples in t.
    double xmin;        ///< X start.
    double xmax;        ///< X finish.
    double ymin;        ///< Y start.
    double ymax;        ///< Y finish.
    double zmin;        ///< Z start.
    double zmax;        ///< Z finish.
    double tmin;        ///< T start.
    double tmax;        ///< T finish.
    std::string outfile;     ///< Output file name.
    std::string fieldObject; ///< Name of field to query.
    
    /// Constructor
    Query();
    /// Reset
    void clear();
    /// Print some properties
    void print()const;
    /// Set methods by property name and value
    template <typename T>
    void set_value(std::string property, T value);

  private:
    /// publish members
    void PublishMembers();
  };
  
  template <typename T>
    void Query::set_value(std::string property, T value)
    {
#ifdef BDSDEBUG
      std::cout << "query> Setting value " << std::setw(25) << std::left << property << value << std::endl;
#endif
      // member method can throw runtime_error, catch and exit gracefully
      try {
	set(this,property,value);
      }
      catch(const std::runtime_error&) {
	std::cerr << "Error: query> unknown option \"" << property << "\" with value " << value  << std::endl;
	exit(1);
      }
    }
}

#endif
