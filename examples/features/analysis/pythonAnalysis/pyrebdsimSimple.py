import pybdsim
import numpy as np
import ROOT

"""
The same as pyrebdsim.py but without comments and minimal code.
"""

def AnalyseFile(filename, outfilename):
    d = pybdsim.Data.Load(filename)
    
    h = ROOT.TH1D("BaseEventDuration",
                  "BaseEvent Durations",
                  50, 0, 10) # 50 bins from 0 to 10

    accH = ROOT.HistogramAccumulator(h,str(h.GetName()).strip('Base'),str(h.GetTitle()).strip('Base'))

    eventTree = d.GetEventTree()
    for event in eventTree:
        h.Reset()
        h.Fill(event.Summary.durationCPU)
        accH.Accumulate(h)
    
    resultHist = accH.Terminate()
    
    outfile = pybdsim.Data.CreateEmptyRebdsimFile(outfilename, d.header.nOriginalEvents)
    pybdsim.Data.WriteROOTHistogramsToDirectory(outfile, "Event/PerEntryHistograms", [resultHist])
    outfile.Close()
       
if __name__ == "__main__":
    AnalyseFile("../../data/sample*.root","pyrebdsimTestSimple.root")
