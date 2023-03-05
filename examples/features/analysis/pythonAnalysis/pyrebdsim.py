import pybdsim
import ROOT

def AnalyseFile(filename, outfilename):
    # load the data
    d = pybdsim.Data.Load(filename)
    # loading data in pybdsim forces the load of the rebdsim
    # shared library and gives us all the rebdsim classes inside
    # ROOT as python functions
    
    # define a histogram
    # we prefix these with 'Base' as we'll use them as a base for
    # mean histograms in an 'accumulator'
    h = ROOT.TH1D("EventDurationBASE",
                  "Event Durations",
                  50, 0, 10) # 50 bins from 0 to 10

    # we now wrap our histograms in an 'accumulator' which
    # builds the mean histogram across events
    # [:-4] to strip off the last 4 characters - ie "BASE"
    # only the first argument, the object name, must be unique
    accH = ROOT.HistogramAccumulator(h,str(h.GetName())[:-4],str(h.GetTitle()))
    # top tip - name the accumulators something different to avoid confusion

    eventTree = d.GetEventTree()
    for event in eventTree:
        # the 'event' temporary object here, now looks exactly
        # like the file layout you see in a TBrowser

        # first of all we empty our base histogram for this event
        h.Reset()

        # fill our histograms - could be some custom if else logic here
        # or looping over N sampler hits or M trajectories
        # this example just has one number per event we're filling
        h.Fill(event.Summary.durationCPU)

        # after filling we accumulate the mean
        accH.Accumulate(h)

    # done with looping over events
    # terminate mean histograms
    resultHist = accH.Terminate()

    # create a rebdsim structured file and put the histogram in it
    # we do this so we can then use rebdsimCombine to merge many of
    # these custom analysis files
    outfile = pybdsim.Data.CreateEmptyRebdsimFile(outfilename, d.header.nOriginalEvents)
    pybdsim.Data.WriteROOTHistogramsToDirectory(outfile, "Event/PerEntryHistograms", [resultHist])
    outfile.Close()
        

if __name__ == "__main__":
    AnalyseFile("../../data/sample*.root","pyrebdsimTest.root")
