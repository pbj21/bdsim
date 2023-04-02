# certain test options that can be added
# For overlap checking (needs option, checkOverlaps=1)
set(OVERLAP_CHECK "Overlap is detected")

# always test in batch mode
set(TESTING_PERM_ARGS --batch)

# special flag to remove SIGINT handling - cmake doesn't pass this on so you can't
# kill a job! Long standing bug in cmake
set(TESTING_PERM_ARGS ${TESTING_PERM_ARGS} --ignoresigint)

add_subdirectory(airwatertarget)
add_subdirectory(atf2)
add_subdirectory(diamond)
add_subdirectory(features)
add_subdirectory(fodoRing)
add_subdirectory(lhc)
add_subdirectory(simpleMachine)
add_subdirectory(beamDump)
add_subdirectory(beamLoss)
add_subdirectory(collimation)
add_subdirectory(target)


if(USE_AWAKE)
  add_subdirectory(AWAKE)
endif()

