{
  gROOT->ProcessLine(".x loadLibraries.C");

  DataLoader* dl = new DataLoader("../../data/sample1.root");

  TTree* evtT = dl->GetEventTree();
  Event* evt = dl->GetEvent();

  evtT->GetEntry(0);

  // copy the data into another event instance - tests copying
  Event* evt2 = new Event();
  evt2->Fill(evt);
  
  return 0;
}
