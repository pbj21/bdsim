import matplotlib.pyplot as plt
import pybdsim

def MakePlots(suffix="", suffixTitle="", savePlots=False):

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
        dn = pybdsim.Field.Load(name+suffix+"_normal.dat")
        ds = pybdsim.Field.Load(name+suffix+"_scaled.dat")
        pybdsim.Field.Plot2DXY(dn, title=title+" Normal"+suffixTitle)
        if savePlots:
            plt.savefig(name+suffix+"_normal.pdf")
        pybdsim.Field.Plot2DXY(ds, title=title+" Scaled"+suffixTitle)
        if savePlots:
            plt.savefig(name+suffix+"_scaled.pdf")
         

if __name__ == "__main__":
    MakePlots(savePlots=True)
    MakePlots("_old", suffixTitle=" Old", savePlots=True)
