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
#include "BDSOutputROOTEventHeader.hh"
#include "Header.hh"
#include "HeaderAnalysis.hh"

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

#include <set>
#include <stdexcept>
#include <string>
#include <vector>

HeaderAnalysis::HeaderAnalysis(const std::vector<std::string>& filenamesIn,
                               Header* headerIn,
                               TChain* chainIn):
  filenames(filenamesIn),
  header(headerIn),
  chain(chainIn)
{;}

HeaderAnalysis::~HeaderAnalysis() noexcept
{;}

unsigned long long int HeaderAnalysis::CountNOriginalEvents(unsigned long long int& nEventsInFileIn,
                                                            unsigned long long int& nEventsInFileSkippedIn,
                                                            unsigned long long int& nEventsRequestedIn)
{
  // We should only read one entry per header tree per file, so we don't double count, the number
  // of events in a file. The header can have 2 entries (start of file and end or run). Use the
  // tree number set to cache this. We could be analysing 1 file with 1 or 2 header entries or a
  // chain of files with 1 or 2 header entries.
  std::set<Int_t> treeFiles;
  unsigned long long int nOriginalEvents = 0;
  for (int i = 0; i < chain->GetEntries(); i++) // assumes 1 header entry per file - fine
    {
      chain->GetEntry(i);
      Int_t currentTreeNumber = chain->GetTreeNumber();

      // Here we take advantage of the fact that although in a given file there might be 2
      // header entries, the first one will always be empty (set to 0) for these two variables.
      // Therefore, we can safely *always* add them to a total.
      nEventsInFileIn += header->header->nEventsInFile;
      nEventsInFileSkippedIn += header->header->nEventsInFileSkipped;
      nEventsRequestedIn += header->header->nEventsRequested;

      if (treeFiles.count(currentTreeNumber) > 0)
        {continue;} // we've processed this file in the chain -> ignore

      if (header->header->nOriginalEvents > 0)
        {nOriginalEvents += header->header->nOriginalEvents;}
      else
        {// not a skimmed file, so nOriginalEvents in header is 0 -> get info from that individual file
          TFile* ftemp;
          try
            {ftemp = new TFile(filenames[i].c_str());}
          catch (const std::exception& e)
            {continue;}
          TTree* eventTree = (TTree*)ftemp->Get("Event");
          if (!eventTree)
            {
              ftemp->Close();
              delete ftemp;
              continue;
            }
          Long64_t nentries = eventTree->GetEntries();
          if (nentries > 0)
            {nOriginalEvents += (unsigned long long int)nentries;}
          ftemp->Close();
          delete ftemp;
        }
      treeFiles.insert(currentTreeNumber);
    }
  return nOriginalEvents;
}
