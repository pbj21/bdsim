import matplotlib.pyplot as plt
import pybdsim
import sys

def MakePlot(filename):

    d = pybdsim.Data.Load(filename)
    h = d.histogramspy["Event/PerEntryHistograms/XYDistribution"]
    pybdsim.Plot.Histogram2D(h, xlabel="X (m)", ylabel="Y (m)")
    plt.tight_layout()
    plt.savefig("xy-distribution.png", dpi=400)


if __name__ == "__main__":
    MakePlot(sys.argv[1])

# python plotXY.py r1_ana.root
