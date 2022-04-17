set(TESTING_ARGS "--outfile=muon_e-")
simple_testing(muon_e- "--file=muon_e-.gmad" ${OVERLAP_CHECK})

set(TESTING_ARGS "--outfile=muon_e+")
simple_testing(muon_e+ "--file=muon_e+.gmad" ${OVERLAP_CHECK})

simple_testing(muon-splitting-kaonplus        "--file=muon_splitting_kaonplus_decay.gmad" "")
simple_testing(muon-splitting-piminus         "--file=muon_splitting_piminus_decay.gmad" "")
simple_testing(muon-splitting-piplus          "--file=muon_splitting_piplus_decay.gmad" "")
simple_testing(muon-splitting-piplus-biased   "--file=muon_splitting_piplus_decay_biased.gmad" "")
simple_testing(muon-splitting-proton-all      "--file=muon_splitting_proton_all_phys.gmad" "")
simple_testing(muon-splitting-exclude-W1      "--file=muon_splitting_proton_all_phys_excludeW1.gmad" "")
simple_testing(muon-splitting-ek-threshold    "--file=muon_splitting_ekthreshold.gmad" "")
simple_testing(muon-splitting-2factor-ek-threshold  "--file=muon_splitting_2factor_ekthreshold.gmad" "")


if (G4_MINOR_VERSION GREATER 2)
  simple_testing(muon-splitting-eplus           "--file=muon_splitting_eplus_annihilation.gmad" "")
  simple_testing(muon-splitting-eplus-biased    "--file=muon_splitting_eplus_annihilation_biased.gmad" "")
endif()
