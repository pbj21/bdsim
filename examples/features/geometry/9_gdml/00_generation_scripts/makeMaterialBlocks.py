import pyg4ometry as pg
import pyg4ometry.geant4 as g4
import pyg4ometry.gdml as gd
import pyg4ometry.visualisation as vi


def MakeModel(vis=True):
    reg = g4.Registry()

    materials = ["airbdsim",
                 "G4_AIR",
                 "G4_BRASS",
                 "G4_C",
                 "G4_GRAPHITE",
                 "G4_Cl",
                 "concrete",
                 "lhcconcrete",
                 "G4_CONCRETE",
                 "G4_Cu",
                 "G4_Fe",
                 "kapton",
                 "G4_KAPTON",
                 "G4_Pb",
                 "stainlesssteel",
                 "G4_STAINLESS-STEEL",
                 "G4_WATER"]

    bl = 30
    bw = 300
    safety = 1e-3

    nBlocks = len(materials)

    box = g4.solid.Box("box",bw,bw,bl,reg)

    wm = g4.MaterialPredefined("G4_Galactic")
    worldLength = nBlocks * (bl + safety) * 1.1
    world = g4.solid.Box("world_solid", 1.1*bw, 1.1*bw, nBlocks*1.1*bl, reg)
    worldLV = g4.LogicalVolume(world, wm, "world_lv", reg)

    position = -0.5*worldLength + 0.5*bl + safety

    for material in materials:
        mat = g4.MaterialArbitrary(material)
        lv  = g4.LogicalVolume(box, mat, material+"_lv", reg)
        pv  = g4.PhysicalVolume([0,0,0], [0,0,position], lv, material+"_pv", worldLV, reg)
        position += bl + safety

    reg.setWorld(worldLV.name)

    w = gd.Writer()
    w.addDetector(reg)
    w.write("../gdmls/colourful_materials.gdml")
    w.writeGmadTester("../14_gdml_colours/14b_gdml_material_colours.gmad","../gdmls/colourful_materials.gdml")

    if vis: 
        v = vi.VtkViewer()
        v.addLogicalVolume(reg.getWorldVolume())
        v.view()
                



if __name__ == "__main__":
    MakeModel(False)
