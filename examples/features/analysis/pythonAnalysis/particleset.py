import pybdsim
import ROOT

def DeterminePDGSet(filename="../../data/shower.root"):
    """
    Print out the set of particle PDG IDs
    from the "c1" sampler of data file.

    Just for example to show some PDGs of interest.
    """
    d = pybdsim.Data.Load(filename)
    pdgIDs = set()
    for event in d.GetEventTree():
        for i in range(event.c1.n):
            pdgIDs.add(event.c1.partID[i])
    print(pdgIDs)

def AnalyseFile(filename, outfilename):
    # load the data
    d = pybdsim.Data.Load(filename)

    # PDG ID to bin number
    particles = {11  : 0,
                 -11 : 1,
                 22  : 2,
                 211 : 3,
                 -211 : 4,
                 310  : 5,
                 321  : 5,
                 2112 : 6,
                 2212 : 7
    } #etc
                  
    h = ROOT.TH1D("ParticlesBASE", "Particles Base", 20, 0, 20)

    # this class comes from librebdsim which gets loaded when we load our data
    accH = ROOT.HistogramAccumulator(h,str(h.GetName())[:-4],str(h.GetTitle()))

    eventTree = d.GetEventTree()
    for event in eventTree:
        h.Reset()

        for i in range(event.c1.n):
            pid = event.c1.partID[i]
            binIndex = particles[pid] if pid in particles else -1
            h.Fill(binIndex, event.c1.weight[i])

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
    AnalyseFile("../../data/shower.root", "pyParticleSetTest.root")
