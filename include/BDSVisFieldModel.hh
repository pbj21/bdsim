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
#ifndef BDSVISFIELDMODEL_H
#define BDSVISFIELDMODEL_H
#include "BDSFieldQueryInfo.hh"

#include "G4Types.hh"
#include "G4VModel.hh"

#include <vector>

class BDSFieldQueryInfo;
class G4Field;
class G4VisAttributes;

class BDSVisFieldModel: public G4VModel
{
public:
  explicit BDSVisFieldModel(const std::vector<BDSFieldQueryInfo*>& queriesIn);
  
  BDSVisFieldModel(const BDSVisFieldModel&) = delete;
  BDSVisFieldModel& operator=(const BDSVisFieldModel&) = delete;
  
  virtual ~BDSVisFieldModel();

  // The main task of a model is to describe itself to the graphics scene.
  virtual void DescribeYourselfTo(G4VGraphicsScene& sceneHandler);

private:
  /// Counter variable as we need to construct a new one of this class and have
  /// it have a unique name.
  static G4int instanceCounter;
  
  /// Return the minimum of 0.8 x the step length in x,y,z
  G4double CalculateArrowLength(const BDSFieldQueryInfo* query) const;
  
  /// Query Info Length. For one dimension, return 0.8 x step size
  G4double QIL(const BDSFieldQueryInfo::QueryDimensionInfo& qi) const;
  
  const std::vector<BDSFieldQueryInfo*> queries;
  
  /// @{ The vis attributes for a zero-field marker.
  G4VisAttributes* pointVisB;
  G4VisAttributes* pointVisE;
  /// @}
};

#endif
