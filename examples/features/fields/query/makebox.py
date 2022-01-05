import pyg4ometry.gdml as _gd
import pyg4ometry.geant4 as _g4
import pyg4ometry.visualisation as _vi

def MakeBox(vis=False):
    reg = _g4.Registry()
    
    galactic = _g4.MaterialPredefined("G4_Galactic") 
    gold = _g4.MaterialPredefined("G4_Au")

    # solids
    cs = _g4.solid.Box("container_solid",500,500,100,reg)
    bs = _g4.solid.Box("box_solid",499.9,499.9,99.9,reg)
        
    # structure 
    wl = _g4.LogicalVolume(cs, galactic, "container_lv", reg)
    bl = _g4.LogicalVolume(bs, gold, "box_lv", reg)
    bp = _g4.PhysicalVolume([0,0,0],[0,0,0],  bl, "box_pv", wl, reg) 
    reg.setWorld(wl)
    
    # gdml output 
    w = _gd.Writer()
    w.addDetector(reg)
    w.write("abox.gdml")

    # visualisation
    if vis:
        v = _vi.VtkViewerColouredMaterial()
        v.addLogicalVolume(wl)
        v.view()
        
if __name__ == "__main__":
    MakeBox()
