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
#include "BinSpecification.hh"
#include "HistogramDef4D.hh"

ClassImp(HistogramDef4D)

HistogramDef4D::HistogramDef4D():
  eBinning(BinSpecification())
{;}

HistogramDef4D::HistogramDef4D(const std::string&      treeNameIn,
                               const std::string&      histNameIn,
                               const BinSpecification& xBinningIn,
                               const BinSpecification& yBinningIn,
                               const BinSpecification& zBinningIn,
                               const BinSpecification& eBinningIn,
                               const std::string&      variableIn,
                               const std::string&      eScaleIn,
                               const std::string&      selectionIn,
                               bool                    perEntryIn):
  HistogramDef3D(treeNameIn, histNameIn, xBinningIn, yBinningIn, zBinningIn, variableIn, selectionIn, perEntryIn),
  eBinning(eBinningIn),
  eScale(eScaleIn)
{
  nDimensions = 4;
}

HistogramDef4D::~HistogramDef4D()
{;}
