import pyg4ometry as pg
from pyg4ometry.gdml import Constant


def MakeLNGeometry(vis=True):
    """
    Make two GDML files with extruded solids with the
    letters L and N in each of them. Used so they can
    be placed close to each other and we can put a beam
    pipe through the geometry to test that we succesfully
    make assembly volumes and strip off the container.
    """

    reg = pg.geant4.Registry()

    w = Constant("w",500,reg) # width
    h = Constant("h",500,reg) # height
    t = Constant("t",50, reg) # thickness

    polygonL = [ [-0.5*w, -0.5*h],
                 [-0.5*w,  0.5*h],
                 [-0.35*w, 0.5*h],
                 [-0.35*w, -0.35*h],
                 [ 0.5*w,  -0.35*h],
                 [ 0.5*w,  -0.5*h] ]

    nL = -0.3*w
    nB = -0.3*w
    polygonN = [ [nL, nB],
                 [nL, 0.5*h],
                 [-0.05*w, 0.5*h],
                 [0.35*w,  -0.05*h],
                 [0.35*w,  0.5*h],
                 [0.5*w,   0.5*h],
                 [0.5*w,   nB],
                 [0.3*w,   nB],
                 [-0.15*w, 0.35*h],
                 [-0.15*w, nB] ]

    slices = [[-0.5*t,[0,0],1.0], [0.5*t,[0,0],1.0]]

    worldSolid = pg.geant4.solid.Box("world-solid",
                                     3*w,3*w,3*w,reg)
    solidL = pg.geant4.solid.ExtrudedSolid("l-solid",
                                           polygonL,
                                           slices,
                                           reg)
    solidN = pg.geant4.solid.ExtrudedSolid("n-solid",
                                           polygonN,
                                           slices,
                                           reg)


    iron = pg.geant4.MaterialPredefined("G4_Fe")
    copper = pg.geant4.MaterialPredefined("G4_Cu")
    worldLV = pg.geant4.LogicalVolume(worldSolid, iron, "world-lv", reg)
    LLV = pg.geant4.LogicalVolume(solidL, copper, "l-lv", reg)
    NLV = pg.geant4.LogicalVolume(solidN, copper, "n-lv", reg)

    pg.geant4.PhysicalVolume([0,0,0],[0,0,0], LLV, "l-pv", worldLV, reg)
    pg.geant4.PhysicalVolume([0,0,0],[0,0,0], NLV, "n-pv", worldLV, reg)

    # now make an assembly of LN and place that
    lnAV = pg.geant4.AssemblyVolume("ln-assembly", reg)
    pg.geant4.PhysicalVolume([0,0,0],[0,0,0], LLV, "l-av-pv", lnAV, reg)
    pg.geant4.PhysicalVolume([0,0,0],[0,0,0], NLV, "n-av-pv", lnAV, reg)

    # now make imprints of the assembly on the worl
    pg.geant4.PhysicalVolume([0,0.1,0], [0,0.2*t,2*t],  lnAV, "ln-left",  worldLV, reg)
    pg.geant4.PhysicalVolume([0,-0.1,0],[0,0.2*t,4*t], lnAV, "ln-right", worldLV, reg)
    pg.geant4.PhysicalVolume([0,-0.1,-1.5],[-0.4*t,-0.2*t,-2*t], lnAV, "ln-right2", worldLV, reg)
    
    reg.setWorld(worldLV)

    w = pg.gdml.Writer()
    w.addDetector(reg)
    w.write("ln.gdml")

    if vis:
        v = pg.visualisation.VtkViewerColouredMaterial()
        v.addLogicalVolume(reg.getWorldVolume())
        v.view()
