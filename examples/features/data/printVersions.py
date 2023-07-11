import ROOT
import glob

fs = glob.glob("*.root")
for f in fs:
    fl = ROOT.TFile(f)
    header = fl.Get("Header")
    print("File:",f)
    header.Scan("bdsimVersion")
    fl.Close()
