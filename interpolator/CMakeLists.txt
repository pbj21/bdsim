# Configure source files
string(TIMESTAMP CURRENT_YEAR %Y)
configure_file(${CMAKE_SOURCE_DIR}/interpolator/bdsinterpolator.cc ${CMAKE_BINARY_DIR}/interpolator/bdsinterpolator.cc @ONLY)

# Build executable and link against needed libraries
add_executable(interpolatorexec ${CMAKE_BINARY_DIR}/interpolator/bdsinterpolator.cc)
set_target_properties(interpolatorexec PROPERTIES OUTPUT_NAME "bdsinterpolator" VERSION ${BDSIM_VERSION})
target_link_libraries(interpolatorexec ${BDSIM_LIB_NAME} ${GMAD_LIB_NAME} ${CLHEP_LIBRARIES} ${GEANT4_LIBRARIES})

# Installation
bdsim_install_targets(interpolatorexec)

get_target_property(interpolatorBinaryName interpolatorexec OUTPUT_NAME)
set(interpolatorBinary ${CMAKE_CURRENT_BINARY_DIR}/${interpolatorBinaryName} CACHE STRING "interpolator binary")
mark_as_advanced(interpolatorBinary)
