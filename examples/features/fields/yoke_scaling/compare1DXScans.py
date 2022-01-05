import matplotlib.pyplot as plt
import numpy as np
import pybdsim

def Make1DPlots(suffixTitle="", savePlots=False):
    names = ['out_xscan_bend',
             'out_xscan_quad',
             'out_xscan_bend_lhc',
             'out_xscan_quad_lhc',
             'out_xscan_sext']
    
    titles = ["Dipole",
              "Quadrupole",
              "LHC Dipole",
              "LHC Quadrupole",
              "Sextupole"]
    
    
    for name,title in zip(names, titles):
        do = pybdsim.Field.Load(name+"_old.dat")
        dn = pybdsim.Field.Load(name+".dat")

        # assume 1D data, so [x,bx,by,bz] for each value in the array
        magso = np.hypot(do.data[:,1],do.data[:,2]).flatten()
        magsn = np.hypot(dn.data[:,1],dn.data[:,2]).flatten()

        xo = do.data[:,0].flatten()
        xn = dn.data[:,0].flatten()
        
        xmin = min([np.min(xo), np.min(xn)])
        xmax = max([np.max(xo), np.max(xn)])

        plt.figure()
        plt.plot(xo, magso, label="Old")
        plt.plot(xn, magsn, label="New")
        plt.yscale('log')
        plt.legend()
        cylim = list(plt.ylim())
        cylim[0] = max(1e-2, cylim[0])
        plt.ylim(*cylim)
        plt.xlabel('X (cm)')
        plt.ylabel("|$B_{x,y}$| (T)")
        plt.title(title+suffixTitle)
        if savePlots:
            plt.savefig(name+"_xscan.pdf")
        plt.close()
         

if __name__ == "__main__":
    Make1DPlots(savePlots=True)
