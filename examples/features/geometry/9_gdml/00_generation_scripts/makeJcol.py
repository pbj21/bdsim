import pyg4ometry.gdml as _gd
import pyg4ometry.geant4 as _g4
import pyg4ometry.visualisation as _vi

def MakeJColGDML(vis=False):
    reg = _g4.Registry()

    # general dimensions in mm
    activeLength = 600
    taperLengthIncoming = 200
    taperLengthOutgoing = 100
    jawThickness        = 100
    jawHeight           = 100
    margin              = 0.1
    
    jawLHalfGap = 12
    jawRHalfGap = 20

    dz = 0.5*(taperLengthIncoming - taperLengthOutgoing)

    halfpi = _gd.Constant("halfpi", "0.5*pi", reg)

    # tapered jaw in plan view - rhombus
    x1 = -0.5*activeLength
    x2 =  x1 - taperLengthIncoming
    x3 = -x1 + taperLengthOutgoing
    x4 = -x1
    y1 = 0
    y2 = jawThickness
    polygon = [[x1,y1], [x2,y2], [x3,y2], [x4,y1]]
    slices  = [[-0.5*jawHeight,[0,0],1], [0.5*jawHeight,[0,0],1]]
    jawSolid = _g4.solid.ExtrudedSolid("jaw_solid", polygon, slices, reg)

    # container solid = "world"
    worldXSize = 2 * max(jawLHalfGap, jawRHalfGap) + 2*jawThickness + 10*margin
    worldYSize = jawHeight + 10*margin
    worldZSize = activeLength + taperLengthIncoming + taperLengthOutgoing + 2*margin
    worldSolid = _g4.solid.Box("world_solid",worldXSize, worldYSize, worldZSize, reg)
    
    galactic = _g4.MaterialPredefined("G4_Galactic") 
    graphite = _g4.MaterialPredefined("G4_GRAPHITE")
        
    # logical volumes
    wLV   = _g4.LogicalVolume(worldSolid, galactic, "world_lv", reg)
    jawLV = _g4.LogicalVolume(jawSolid, graphite, "jaw_lv", reg)

    # placements
    leftJawPV  = _g4.PhysicalVolume([-halfpi, 0, -halfpi],[-jawLHalfGap,0,dz], jawLV, "left_jaw_pv",  wLV, reg)
    rightJawPV = _g4.PhysicalVolume([halfpi,  0,  halfpi],[ jawRHalfGap,0,dz], jawLV, "right_jaw_pv", wLV, reg) 
    
    # set world volume
    reg.setWorld(wLV.name)

    # check overlaps
    wLV.checkOverlaps(recursive=True)
    
    # gdml output 
    w = _gd.Writer()
    w.addDetector(reg)
    w.write("../gdmls/jcol.gdml")

    # visualisation
    if vis: 
        v = _vi.VtkViewer()
        v.addLogicalVolume(reg.getWorldVolume())
        v.view()


if __name__ == "__main__":
    MakeJColGDML()
