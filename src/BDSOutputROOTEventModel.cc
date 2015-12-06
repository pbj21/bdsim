#include "BDSOutputROOTEventModel.hh"

ClassImp(BDSOutputROOTEventModel);

BDSOutputROOTEventModel::BDSOutputROOTEventModel()
{
}

BDSOutputROOTEventModel::~BDSOutputROOTEventModel()
{
}

void BDSOutputROOTEventModel::Fill()
{
  // get accelerator model
  BDSBeamline *beamline = BDSAcceleratorModel::Instance()->GetFlatBeamline();

  // iterate over flat beamline
  for(auto i = beamline->begin(); i != beamline->end(); ++i)
  {
    // Name
    this->name.push_back((*i)->GetName());
    // Length
    this->length.push_back((float &&) (*i)->GetAcceleratorComponent()->GetArcLength());

    // Positions
    G4ThreeVector p;
    p = (*i)->GetPositionStart();
    this->staPos.push_back(TVector3(p.getX(),p.getY(),p.getZ()));
    p = (*i)->GetPositionMiddle();
    this->midPos.push_back(TVector3(p.getX(),p.getY(),p.getZ()));
    p = (*i)->GetPositionEnd();
    this->endPos.push_back(TVector3(p.getX(),p.getY(),p.getZ()));

    // Rotations
    G4RotationMatrix *gr;
    gr = (*i)->GetRotationStart();
    TRotation rr = TRotation();
    rr.SetXEulerAngles(gr->getTheta(),gr->getPhi(),gr->getPsi());
    this->staRot.push_back(rr);
    gr = (*i)->GetRotationMiddle();
    rr.SetXEulerAngles(gr->getTheta(),gr->getPhi(),gr->getPsi());
    this->midRot.push_back(rr);
    gr = (*i)->GetRotationEnd();
    rr.SetXEulerAngles(gr->getTheta(),gr->getPhi(),gr->getPsi());
    this->endRot.push_back(rr);

    // Reference orbit positions
    p = (*i)->GetReferencePositionStart();
    this->staPos.push_back(TVector3(p.getX(),p.getY(),p.getZ()));
    p = (*i)->GetReferencePositionMiddle();
    this->midPos.push_back(TVector3(p.getX(),p.getY(),p.getZ()));
    p = (*i)->GetReferencePositionEnd();
    this->endPos.push_back(TVector3(p.getX(),p.getY(),p.getZ()));

    // Reference orbit rotations
    gr = (*i)->GetReferenceRotationStart();
    rr.SetXEulerAngles(gr->getTheta(),gr->getPhi(),gr->getPsi());
    this->staRot.push_back(rr);
    gr = (*i)->GetReferenceRotationMiddle();
    rr.SetXEulerAngles(gr->getTheta(),gr->getPhi(),gr->getPsi());
    this->midRot.push_back(rr);
    gr = (*i)->GetReferenceRotationEnd();
    rr.SetXEulerAngles(gr->getTheta(),gr->getPhi(),gr->getPsi());
    this->endRot.push_back(rr);

  }
}
