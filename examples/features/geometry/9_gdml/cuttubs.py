import os as _os
import pyg4ometry.gdml as _gd
import pyg4ometry.geant4 as _g4
import pyg4ometry.visualisation as _vi

import math
import numpy as np

def MakeCutTubs(vis=False, interactive=False, angle=np.pi/6.0):
    reg = _g4.Registry()
    
    wm = _g4.Material(name="G4_Galactic")


    # work out components of unit vector
    dx = 1.0*math.sin(0.5*angle)
    dz = 1.0*math.cos(0.5*angle)
    
    ws = _g4.solid.CutTubs("ts", 0, 500, 1200, 0, 2*np.pi, [-dx, 0, -dz], [-dx, 0, dz], reg)
    wl = _g4.LogicalVolume(ws, wm, "wl", reg)
    reg.setWorld(wl)
    
    # gdml output 
    w = _gd.Writer()
    w.addDetector(reg)
    w.write("cuttubs.gdml")

    # visualisation
    v = None
    if vis: 
        v = _vi.VtkViewer()
        v.addLogicalVolume(reg.getWorldVolume())
        v.view(interactive=interactive)

if __name__ == "__main__":
    MakeCutTubs()
