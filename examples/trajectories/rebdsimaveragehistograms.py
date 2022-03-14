import pybdsim
import ROOT

def Analyse(filename, outputfilename):
    d = pybdsim.Data.Load(filename)

    particles    = [13,-13,14,-14]
    particlesStr = [str(p) for p in particles]
    names        = [r'$\mu^-$',     r'$\mu^+$',
                    r'$\nu_{\mu}$', r'$\overline{\nu}_{\mu}$']
    
    # setup histograms - 1x 2D histogram in X,Z for each particle
    # a base hsitogram is one we will use for each event and empty it afterwards
    baseHistograms = {}
    for p,ps in zip(particles,particlesStr):
        # x dimension of histogram: 50 bins in Z from 0 to 10m
        # y dimension of histogram: 50 bins in X from -1 to 1 m
        # note the histogram has no concept of units - it's purely just a
        # number so we need to choose ranges appropriately
        h = ROOT.TH2D("Origin_ZX_"+ps+"BASE", "Origin of "+ps, 50, 0, 10, 50, -0.5, 0.5)
        baseHistograms[p] = h

    # make accumulators that calculate a rolling average for each histogram
    accumulators = {}
    for p,hist in baseHistograms.items():
        # note we must have a different name for the resultant accumulated histogram, so we strip
        # off the 'BASE' suffix we added (knowingly) to the base histogram
        accumulators[p] = ROOT.HistogramAccumulator(hist, hist.GetName().strip("BASE"), hist.GetTitle())

    eventTree = d.GetEventTree()
    # using this python syntax for name in thing, ROOT will make the variable
    # 'name' ('event') here, have the exact same layout as we see in a TBrowser
    # and as is in the file.
    for event in eventTree:
        for p,h in baseHistograms.items():
            h.Reset()
        
        # now loop over the trajectories stored for this event
        for i in range(event.Trajectory.n):
            # get the particle ID of the trajectory
            partID = event.Trajectory.partID[i]
            if partID not in particles:
                continue # skip ones we don't want to histogram

            # i-th trajectory and the 0th (ie first) point of the trajectory position vector
            originPos = event.Trajectory.XYZ[i][0]
            x,z = originPos[0],originPos[2]
            weight = event.Trajectory.postWeights[i][0]

            baseHistograms[partID].Fill(z, x, weight)

        # at the end of the event we 'accumulate' one event
        for p,h in baseHistograms.items():
            accumulators[p].Accumulate(h)

    # now we terminate the accumulators - ie normalise to the number of events
    results = {}
    for p,h in accumulators.items():
        results[p] = h.Terminate()

    # now write histograms out to a new ROOT file
    f = ROOT.TFile(outputfilename, "RECREATE")
    for p,h in results.items():
        h.AddDirectory(True) # bdsim / rebdsim classes turn off AddDirectory by default
        h.Write()
    f.Close()
