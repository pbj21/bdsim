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
#ifndef HISTOGRAMDEF_H
#define HISTOGRAMDEF_H

#include <string>

#include "Rtypes.h" // for classdef

/**
 * @brief Common specification for a histogram.
 *
 * @author L. Nevay
 */

class HistogramDef
{
public:
  /// Public constructor only for compatibility with ROOT - not intended for use.
  HistogramDef();

  /// Use this constructor.
  HistogramDef(const std::string& treeNameIn,
	       const std::string& histNameIn,
	       int                nDimensionsIn,
               const std::string& variable,
	       const std::string& selectionIn = "1",
	       bool               perEnetry   = true);
  virtual ~HistogramDef(){;};

  /// Copy this instance. Virtual to be overridden in derived classes.
  virtual HistogramDef* Clone() const {return new HistogramDef(*this);}
  
  std::string treeName;
  std::string histName;
  int         nDimensions;
  std::string variable;
  std::string selection;
  bool        perEntry;

  ClassDef(HistogramDef, 1);
};

#endif
