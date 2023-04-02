import pyg4ometry.geant4 as _g4
import pyg4ometry.visualisation as _vis
import pyg4ometry.gdml as _gdml

def GenerateSensitive1(view=False):
    reg = _g4.Registry()
    
    # create with box, then two concurrent spheres in line of beam
    # to practice hiearchy for sensitivity
    # let's call this one 'sa'
    saContainerSolid = _g4.solid.Box("saContainerSolid", 200, 200, 200, reg)
    saContainerLV = _g4.LogicalVolume(saContainerSolid, 'G4_Be', 'saContainerLV', reg)

    saOuterSolid = _g4.solid.Orb('saOuterSolid', 100, reg)
    saInnerSolid = _g4.solid.Orb('saInnerSolid',  50, reg)

    saOuterLV = _g4.LogicalVolume(saOuterSolid, 'G4_C',  'saOuterLV', reg)
    saInnerLV = _g4.LogicalVolume(saInnerSolid, 'G4_Ca', 'saInnerLV', reg)

    saOuterPV = _g4.PhysicalVolume([0,0,0],
                                   [0,0,0],
                                   saOuterLV,
                                   'saOuterPV',
                                   saContainerLV,
                                   reg)
    saInnerPV = _g4.PhysicalVolume([0,0,0],
                                   [0,0,0],
                                   saInnerLV,
                                   'saInnerPV',
                                   saOuterLV,
                                   reg)

    reg.setWorld(saContainerLV)

    if view:
        v = _visVtkViewerColouredMaterial()
        v.addDetectr(saContainerLV)
        v.view()
    
    w = _gdml.Writer()
    w.addDetector(reg)
    w.write('sensitive1.gdml')


def GenerateSensitive2(view=False):
    reg = _g4.Registry()
    
    # create with box, then two concurrent spheres in line of beam
    # to practice hiearchy for sensitivity
    # let's call this one 'sb'
    sbContainerSolid = _g4.solid.Box("sbContainerSolid", 200, 200, 200, reg)
    sbContainerLV = _g4.LogicalVolume(sbContainerSolid, 'G4_Be', 'sbContainerLV', reg)

    sbOuterSolid = _g4.solid.Box('sbOuterSolid', 100,100,100, reg)
    sbInnerSolid = _g4.solid.Box('sbInnerSolid',  50, 50, 50, reg)
    sbCordSolid  = _g4.solid.Box('sbCoreSolid',   30, 30, 30, reg)

    sbOuterLV = _g4.LogicalVolume(sbOuterSolid, 'G4_C',  'sbOuterLV', reg)
    sbInnerLV = _g4.LogicalVolume(sbInnerSolid, 'G4_Ca', 'sbInnerLV', reg)
    sbCordLV  = _g4.LogicalVolume(sbCordSolid,  'G4_Fe', 'sbCoreLV',  reg)

    sbOuterPV = _g4.PhysicalVolume([0,0,0],[0,0,0],sbOuterLV,
                                   'sbOuterPV', sbContainerLV, reg)
    sbInnerPV = _g4.PhysicalVolume([0,0,0],[0,0,0],sbInnerLV,
                                   'sbInnerPV', sbOuterLV, reg)
    sbInnerPV = _g4.PhysicalVolume([0,0,0],[0,0,0],sbCordLV,
                                   'sbCorePV', sbInnerLV, reg)

    reg.setWorld(sbContainerLV)

    if view:
        v = _vis.VtkViewerColouredMaterial()
        v.addDetector(saContainerLV)
        v.view()
    
    w = _gdml.Writer()
    w.addDetector(reg)
    w.write('sensitive2.gdml')


def GenerateSensitive3(view=False):
    reg = _g4.Registry()
    
    # create with box, then two concurrent spheres in line of beam
    # to practice hiearchy for sensitivity
    # let's call this one 'sb'
    x = 500
    sbContainerSolid = _g4.solid.Box("sbContainerSolid", x, x, 110, reg)
    sbContainerLV = _g4.LogicalVolume(sbContainerSolid, 'G4_Be', 'sbContainerLV', reg)

    parentLV = sbContainerLV
    for i,z in enumerate([100,90,80,70,60,50]):
        #sol = _g4.solid.Orb("orb_"+str(i), r, reg)
        sol = _g4.solid.Box("box_"+str(i), x-2*i, x-2*i, z, reg)
        lv = _g4.LogicalVolume(sol, 'G4_Fe', "box_"+str(i)+"_lv", reg)
        _g4.PhysicalVolume([0,0,0], [0,0,0], lv, "box_"+str(i)+"_pv", parentLV, reg)
        parentLV = lv

    reg.setWorld(sbContainerLV)

    if view:
        v = _vis.VtkViewerColouredMaterial()
        v.addDetector(saContainerLV)
        v.view()
    
    w = _gdml.Writer()
    w.addDetector(reg)
    w.write('sensitive3.gdml')


if __name__ == "__main__":
    GenerateSensitive1()
    GenerateSensitive2()
    GenerateSensitive3()
