import pybdsim
import matplotlib.pyplot as plt



def PlotDiffractionSpectra(zoom=False):

    off = pybdsim.Data.Load("diffraction-off.root")
    on  = pybdsim.Data.Load("diffraction-on.root")

    name = "Event/SimpleHistograms/ProtonFractionalP"
    if zoom:
        name += "Zoom"
    histOff = off.histogramspy[name]
    histOn  = on.histogramspy[name]

    # use the underlying ROOT->TH1D functions to normalise the simple histogram
    histOff.hist.Scale(1.0/histOff.entries)
    histOff = pybdsim.Data.TH1(histOff.hist) # reconstruct the python bit

    histOn.hist.Scale(1.0/histOn.entries)
    histOn = pybdsim.Data.TH1(histOn.hist) # reconstruct the python bit

    txt = r"0.002" if zoom else r"0.02"
    pybdsim.Plot.Histogram1DMultiple([histOff, histOn],
                                     ["Diffraction Off", "Diffraction On"],
                                     xlabel=r'$\Delta$P/P',
                                     ylabel=r'Fraction of Events / '+txt+r' $\Delta$P',
                                     log=True,
                                    title="Proton Diffraction on Target of 40cm G4_C")
    fn = "proton-diffraction-comparison"
    if zoom:
        plt.xlim(-0.02,0.001)
        fn += "-zoom"
    else:
        plt.xlim(-0.2,0.01)
    fn += ".pdf"
    plt.savefig(fn)


if __name__ == "__main__":
    PlotDiffractionSpectra()
    PlotDiffractionSpectra(zoom=True)
