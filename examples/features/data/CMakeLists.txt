add_subdirectory(dataVersions)


# skimming
add_test(NAME bdskim COMMAND bdskimExec skimselection.txt sample1.root skimmed-s1.root)
add_test(NAME bdskim-default-outfile COMMAND bdskimExec skimselection.txt sample1.root)


# combination of raw data
add_test(NAME bdsimCombine COMMAND bdsimCombineExec combined-raw.root sample1.root sample2.root)
