import pybdsim
import numpy as np
import ROOT

def AnalyseFile(filename, outfilename):
    # load the data
    d = pybdsim.Data.Load(filename)
    # loading data in pybdsim forces the load of the rebdsim
    # shared library and gives us all the rebdsim classes inside
    # ROOT as python functions
    
    # explicitly specify the data type in the numpy array as 'd' for double
    # for compatibility with root
    logBins = np.logspace(-3,1,50,dtype='d')
    hl = ROOT.TH1D("EventDurationLogBASE",
                   "Event Durations",
                   len(logBins)-1,# one less bin than n bin edges
                   logBins)

    # we now wrap our histograms in an 'accumulator' which
    # builds the mean histogram across events
    # only the first argument, the object name, must be unique
    accH = ROOT.HistogramAccumulator(h,str(h.GetName())[:-4],str(h.GetTitle()))
    # top tip - name the accumulators something different to avoid confusion

    eventTree = d.GetEventTree()
    for event in eventTree:
        # WARNING - never construct a numpy array inside the loop
        # it will get progressibly slower and slower (just numpy)
        
        # the 'event' temporary object here, now looks exactly
        # like the file layout you see in a TBrowser

        # first of all we empty our base histogram for this event
        hl.Reset()

        # fill our histograms - could be some custom if else logic here
        # or looping over N sampler hits or M trajectories
        # this example just has one number per event we're filling
        hl.Fill(event.Summary.durationCPU)

        # after filling we accumulate the mean
        accHL.Accumulate(hl)

    # done with looping over events
    # terminate mean histograms
    resultHist = accHL.Terminate()
    
    outfile = pybdsim.Data.CreateEmptyRebdsimFile(outfilename, d.header.nOriginalEvents)
    pybdsim.Data.WriteROOTHistogramsToDirectory(outfile, "Event/PerEntryHistograms", [resultHist])
    outfile.Close()
        
if __name__ == "__main__":
    AnalyseFile("../../data/sample*.root","pyrebdsimTestLog.root")
