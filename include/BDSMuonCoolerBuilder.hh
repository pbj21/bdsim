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
#ifndef BDSMUONCOOLERBUILDER_H
#define BDSMUONCOOLERBUILDER_H

class BDSMuonCooler;
namespace GMAD
{
  struct Element;
}

namespace BDS
{
  /// Build a muon cooler beam line element interpeting and checking
  /// all the relevant parameters from the parser definition.
  BDSMuonCooler* BuildMuonCooler(const GMAD::Element* element);
}

#endif
