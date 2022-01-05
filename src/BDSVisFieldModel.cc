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
#include "BDSColourScaleMagma.hh"
#include "BDSColourScaleViridis.hh"
#include "BDSFieldQueryForVis.hh"
#include "BDSFieldQueryInfo.hh"
#include "BDSVisFieldModel.hh"

#include "G4ArrowModel.hh"
#include "G4Colour.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4VGraphicsScene.hh"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <stdio.h>
#include <string>
#include <vector>

G4int BDSVisFieldModel::instanceCounter = 0;

BDSVisFieldModel::BDSVisFieldModel(const std::vector<BDSFieldQueryInfo*>& queriesIn):
  queries(queriesIn)
{
  fGlobalTag = "BDSVisFieldModel";
  fGlobalDescription = "Field view #" + std::to_string(instanceCounter);
  instanceCounter++;
}

BDSVisFieldModel::~BDSVisFieldModel()
{;}

void BDSVisFieldModel::DescribeYourselfTo(G4VGraphicsScene& sceneHandler)
{
  auto bFieldColour = BDSColourScaleViridis();
  auto eFieldColour = BDSColourScaleMagma();
  
  BDSFieldQueryForVis querier;
  for (const auto& query : queries)
    {
      querier.QueryField(query);
      G4double maxFieldB = querier.MaxFieldB();
      G4double maxFieldE = querier.MaxFieldE();
      const std::vector<std::array<G4double, 9>> xyzBEs = querier.Values();
      
      G4double arrowLength = CalculateArrowLength(query);
      if (arrowLength > 0.01 * std::numeric_limits<G4double>::max()) // This shouldn't happen, but is a sanity check
	{arrowLength = 1e-3 * sceneHandler.GetExtent().GetExtentRadius();}
      
      G4double arrowWidth = 0.3*arrowLength;
      
      if (query->queryMagnetic)
	{
	  G4String arrowPrefix = query->name + "_B_";
	  for (const auto& xyzBE: xyzBEs)
	    {
	      // x y z Bx By Bz Ex Ey Ez
	      G4ThreeVector B(xyzBE[3], xyzBE[4], xyzBE[5]);
	      G4ThreeVector unitB = B.unit();
	      G4double bMag = B.mag();
	      if (bMag == 0)
		{continue;} // don't add 0 field arrows
	      G4ThreeVector midPoint(xyzBE[0], xyzBE[1], xyzBE[2]);
	      G4ThreeVector startPoint = midPoint - 0.5*arrowLength*unitB;
	      G4ThreeVector endPoint = midPoint + 0.5*arrowLength*unitB;
	      G4Colour arrowColour = bFieldColour.GetValue(bMag / maxFieldB);
	      char buf[100];
	      sprintf(buf, "(%.2g, %.2g, %.2g)", xyzBE[0], xyzBE[1], xyzBE[2]);
	      std::string arrowName = buf;
	      G4ArrowModel FArrow(startPoint.x(), startPoint.y(), startPoint.z(),
				  endPoint.x(), endPoint.y(), endPoint.z(),
				  arrowWidth, arrowColour, arrowName);
	      FArrow.DescribeYourselfTo(sceneHandler);
	    }
	}
      
      if (query->queryElectric)
	{
	  G4String arrowPrefix = query->name + "_E_";
	  for (const auto& xyzBE: xyzBEs)
	    {
	      // x y z Bx By Bz Ex Ey Ez
	      G4ThreeVector E(xyzBE[3], xyzBE[4], xyzBE[5]);
	      G4ThreeVector unitE = E.unit();
	      G4double eMag = E.mag();
	      if (eMag == 0)
		{continue;} // don't add 0 field arrows
	      G4ThreeVector midPoint(xyzBE[0], xyzBE[1], xyzBE[2]);
	      G4ThreeVector startPoint = midPoint - 0.5*arrowLength*unitE;
	      G4ThreeVector endPoint = midPoint + 0.5*arrowLength*unitE;
	      G4Colour arrowColour = eFieldColour.GetValue(eMag / maxFieldE);
	      char buf[100];
	      sprintf(buf, "(%.2g, %.2g, %.2g)", xyzBE[0], xyzBE[1], xyzBE[2]);
	      std::string arrowName = buf;
	      
	      G4ArrowModel FArrow(startPoint.x(), startPoint.y(), startPoint.z(),
				  endPoint.x(), endPoint.y(), endPoint.z(),
				  arrowWidth, arrowColour, arrowName);
	      FArrow.DescribeYourselfTo(sceneHandler);
	    }
	}
      
      querier.CleanUp();
    }
}

G4double BDSVisFieldModel::CalculateArrowLength(const BDSFieldQueryInfo* query) const
{
  G4double result = std::min({QIL(query->xInfo), QIL(query->yInfo), QIL(query->zInfo)});
  return result;
}

G4double BDSVisFieldModel::QIL(const BDSFieldQueryInfo::QueryDimensionInfo& qi) const
{
  if (qi.n == 1)
    {return std::numeric_limits<G4double>::max();}
  
  G4double step = std::abs((qi.max - qi.min) / ((G4double)qi.n));
  return 0.8 * step;
}
