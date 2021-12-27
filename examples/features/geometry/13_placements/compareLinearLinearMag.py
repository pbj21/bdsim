import pybdsim

linear = pybdsim.Field.Load("muon-field-interpolated-linear.dat")
linearmag = pybdsim.Field.Load("muon-field-interpolated-linearmag.dat")

pybdsim.Field.Plot2DXY(linear)
pybdsim.Field.Plot2DXY(linearmag)
