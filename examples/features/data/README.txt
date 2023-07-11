# Sample Data
Based on originalmodels/sc.gmad
2 files of 10 events with seeds 123 and 321
sample1.root - seed=123
sample2.root - seed=321
minimal-data.root - seed=123 - similar to sample1 but name different to avoid clashing with globbing in analysis files (sample*)

analysisConfig.txt was used with rebdsim to generate the analysis output files

ana1.root - analysis of sample1.root
ana2.root - anlaysis of sample2.root

combined-ana.root - from rebdsimCombine combined-ana.root ana1.root ana2.root

output.seedstate.txt - example ascii seed state information

fodo.root - generated from originalmodels/fodo.gmad with seed 456, 100 particles
fodo-no-primaries - generated from originalmodels/fodo-no-primaries.gmad with seed 654, 100 particles

optics.root - generated from fodo.root

# Sample Data from a shower (large particle variety)
Based on originalmodels/shower.gmad
1 file of 10 events; seed specified in file; various bits of output turned off to save space

=========================
In directory dataVersions
=========================
caches of previous data versions
sample1-vX.X.root
1 file with 10 events using originalModels/sc.gmad with seed 123
