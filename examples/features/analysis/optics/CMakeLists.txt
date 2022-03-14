rebdsim_optics_test(analysis-optics              ../../data/fodo.root              optics.root)
rebdsim_optics_test(analysis-optics-no-primaries ../../data/fodo-no-primaries.root optics-no-primaries.root)

# manual test for incorrect number of arguments and a fail
add_test(NAME analysis-optics-incorrect-arguments  COMMAND rebdsimOpticsExec)
set_tests_properties(analysis-optics-incorrect-arguments PROPERTIES WILL_FAIL 1)


add_test(NAME analysis-optics-emittance-on-fly     COMMAND rebdsimOpticsExec ../../data/fodo.root opticsEOF.root --emittanceOnFly)
add_test(NAME analysis-optics-emittance-on-the-fly COMMAND rebdsimOpticsExec ../../data/fodo.root opticsEOF.root --emittanceOnTheFly)
add_test(NAME analysis-optics-wrong-argument       COMMAND rebdsimOpticsExec ../../data/fodo.root opticsEOF.root --bananas)
set_tests_properties(analysis-optics-wrong-argument PROPERTIES WILL_FAIL 1)
add_test(NAME analysis-optics-emittance-on-fly-default-output COMMAND rebdsimOpticsExec ../../data/fodo.root --emittanceOnTheFly)

rebdsim_orbit_test(analysis-orbit ../../data/fodo.root orbit.root 3)

# manual test for incorrect number of arguments
add_test(NAME analysis-orbit-incorrect-arguments COMMAND rebdsimOrbitExec)
set_tests_properties(analysis-orbit-incorrect-arguments PROPERTIES WILL_FAIL 1)
