import matplotlib.pyplot as plt
import numpy as np
import pybdsim

def MakeHistograms(suffix, nBins=30, suffixTitle="", savePlots=False):

    names = ['out_sfo_bend',
             'out_sfo_quad',
             'out_sfo_bend_lhc',
             'out_sfo_quad_lhc',
             'out_sfo_sext']
    
    titles = ["Dipole",
              "Quadrupole",
              "LHC Dipole",
              "LHC Quadrupole",
              "Sextupole"]
    
    
    for name,title in zip(names, titles):
        do = pybdsim.Field.Load(name+"_old"+suffix+".dat")
        dn = pybdsim.Field.Load(name       +suffix+".dat")

        # assume 2D data, so [x,y,bx,by,bz] for each value in the array
        magso = np.hypot(do.data[:,:,2],do.data[:,:,3]).flatten()
        magsn = np.hypot(dn.data[:,:,2],dn.data[:,:,3]).flatten()

        xmin = min([np.min(magso), np.min(magsn)])
        xmax = max([np.max(magso), np.max(magsn)])

        plt.figure()
        plt.hist(magso, nBins, range=(xmin,xmax), label="Old", histtype="step")
        plt.hist(magsn, nBins, range=(xmin,xmax), label="New", histtype="step")
        plt.yscale('log')
        plt.legend()
        plt.xlabel("|$B_{x,y}$| (T)")
        plt.ylabel("Count")
        plt.title(title+suffixTitle)
        if savePlots:
            plt.savefig(name+suffix+"_comparison.pdf")
        plt.close()

        # xy radius
        ro = np.hypot(do.data[:,:,0],do.data[:,:,1]).flatten()
        rn = np.hypot(dn.data[:,:,0],dn.data[:,:,1]).flatten()
        rmax = max([np.max(ro), np.max(rn)])

        plt.figure()
        plt.hist(ro, nBins, weights=magso, range=(0,rmax), label="Old", histtype="step")
        plt.hist(rn, nBins, weights=magsn, range=(0,rmax), label="New", histtype="step")
        plt.yscale('log')
        plt.legend()
        plt.xlabel("Radius (cm)")
        plt.ylabel("|$B_{x,y}$| $\cdot$ r (T-cm)")
        plt.title(title+suffixTitle)
        if savePlots:
            plt.savefig(name+suffix+"_radial_comparison.pdf")
        plt.close()
         

if __name__ == "__main__":
    MakeHistograms("_normal", suffixTitle=" Normal", savePlots=True)
    MakeHistograms("_scaled", suffixTitle=" Scaled", savePlots=True)
