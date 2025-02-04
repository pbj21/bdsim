import pyg4ometry as pg
import pyg4ometry.geant4 as g4

def Generate():
    GenerateFile1()
    GenerateFile2()

def GenerateFile1():
    reg = g4.Registry()
    b1s = g4.solid.Box('box', 20, 30, 40, reg)
    b1l = g4.LogicalVolume(b1s, 'G4_Cu', 'boxlv', reg)

    worlds = g4.solid.Box('world', 200, 200, 200, reg)
    worldl = g4.LogicalVolume(worlds, 'G4_Galactic', 'worldlv', reg)

    b1p = g4.PhysicalVolume([0,0,0], [0,0,0],
                            b1l,
                            "boxpv",
                            worldl, reg)

    reg.setWorld('worldlv')
    w = pg.gdml.Writer()
    w.addDetector(reg)
    w.write("../gdmls/box1.gdml")
    

def GenerateFile2():
    reg = g4.Registry()
    # purposively give it the same name to test different file loading
    b2s = g4.solid.Orb('box', 34, reg)
    b2l = g4.LogicalVolume(b2s, 'G4_WATER', 'boxlv', reg)

    worlds = g4.solid.Box('world', 200, 200, 200, reg)
    worldl = g4.LogicalVolume(worlds, 'G4_Galactic', 'worldlv', reg)

    b2p = g4.PhysicalVolume([0,0,0], [0,0,0],
                            b2l,
                            "boxpv",
                            worldl,
                            reg)

    reg.setWorld('worldlv')
    w = pg.gdml.Writer()
    w.addDetector(reg)
    w.write("../gdmls/box2.gdml")


if __name__ == "__main__":
    Generate()
