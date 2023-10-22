# find MadX quietly
find_package(MadX QUIET)

# configure tester with correct BDSIM and MadX executables
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run_bdsimMadx.py.in ${CMAKE_CURRENT_BINARY_DIR}/run_bdsimMadx.py)

#if (MADX_FOUND AND USE_ROBDSIM AND PY_ROOT_NUMPY AND PY_PYBDSIM AND PY_PYMADX)
#  message(STATUS "Tracking tests enabled")
#  message(STATUS "Found MadX: ${MADX_EXECUTABLE}")
#  tracking_test(tracking-fodo       --filepath=1_fodo/FODO.madx)
#  tracking_test(tracking-drift      --filepath=2_drift/DRIFT.madx)
#  tracking_test(tracking-quadrupole --filepath=3_quadrupole/QUAD.madx)
#  tracking_test(tracking-sextupole  --filepath=4_sextupole/SEXT.madx)
#  tracking_test(tracking-dba        --filepath=5_dba/DBA.madx)
#  tracking_test(tracking-rbend      --filepath=6_rbend/RBEND.madx)
#  tracking_test(tracking-sbend      --filepath=7_sbend/SBEND.madx)
#  tracking_test(tracking-octupole   --filepath=9_octupole/OCT.madx)
#  tracking_test(tracking-hkicker    --filepath=10_hkicker/hkicker.madx)
#endif()

