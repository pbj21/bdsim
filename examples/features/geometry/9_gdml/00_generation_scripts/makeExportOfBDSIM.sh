# takes too long for preprocessing as too many volumes
#bdsim --file=../../10_multiple_beamlines/multiplebeamlines.gmad --output=none --exportGeometryTo=../gdmls/multiple_beamlines_w_colour.gdml --batch


bdsim --file=../../../../collimation/collimation.gmad --output=none --exportGeometryTo=../gdmls/collimation_export_w_colour.gdml --batch
