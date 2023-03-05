import pyg4ometry as pg
import pyg4ometry.geant4 as g4

def GenerateFile(option):
    reg = g4.Registry()
    b1s = g4.solid.Box('box', 20, 30, 40, reg)

    if option == 1:
        # "iron" is an alias in BDSIM but this is allowed
        mat = g4.MaterialSingleElement("iron",26,55.8452,7.874,reg) # iron at near room temp
    if option == 2:
        # "weightiron" is a material already defined in bdsim but we're creating
        # a unique conflicting definition here. This can't be an single element
        # name as that would be forwarded to a NIST one that wouldn't test this
        mat = g4.MaterialSingleElement("weightiron",26,55.8452,7.874,reg) # iron at near room temp
    elif option == 3:
        # test a more involved material... both elements and material
        # should conflict with pre-existing ones in bdsim
        # 'air' is an alias fro G4_AIR in bdsim but old air is "bdsimair"
        mat = g4.MaterialCompound("airbdsim",1.290e-3,2,reg)
        ne = g4.ElementSimple("nitrogen","N",7,14.01)
        oe = g4.ElementSimple("oxygen","O",8,16.0)
        mat.add_element_massfraction(ne,0.7)
        mat.add_element_massfraction(oe,0.3)

    
    b1l = g4.LogicalVolume(b1s, mat, 'boxlv', reg)

    worlds = g4.solid.Box('world', 200, 200, 200, reg)
    worldl = g4.LogicalVolume(worlds, 'G4_Galactic', 'worldlv', reg)

    b1p = g4.PhysicalVolume([0,0,0], [0,0,0], b1l, "boxpv", worldl, reg)

    reg.setWorld(worldl)
    w = pg.gdml.Writer()
    w.addDetector(reg)
    w.write("box-w-bdsim-material-conflict-"+str(option)+".gdml")


if __name__ == "__main__":
    GenerateFile(1)
    GenerateFile(2)
    GenerateFile(3)
