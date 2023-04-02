# record which git version is being built

if (GIT_FOUND)
  execute_process(COMMAND           ${GIT_EXECUTABLE} describe --always --dirty
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  OUTPUT_VARIABLE   VERSION_SHA1 
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                  RESULT_VARIABLE   GIT_SHA1_RESULT)
    
  if (GIT_SHA1_RESULT EQUAL "1")
    message(STATUS "Couldn't determine git SHA1 - using ${BDSIM_VERSION} as BDSIM version in output.")
    set(VERSION_SHA1 ${BDSIM_VERSION} PARENT_SCOPE)
  else()
    set(VERSION_SHA1 ${VERSION_SHA1} PARENT_SCOPE) # upgrade the scope of the output variable
  endif()
else()
  set(VERSION_SHA1 ${BDSIM_VERSION} PARENT_SCOPE)
endif()

set(BDSIM_ROOT_INCLUDE_PATH "$BDSIM/include/bdsim/:$BDSIM/include/bdsim/analysis/:$BDSIM/include/bdsim/parser/")
if (USE_HEPMC3)
  set(BDSIM_ROOT_INCLUDE_PATH ${BDSIM_ROOT_INCLUDE_PATH}:${HEPMC3_INCLUDE_DIR})
endif()
if (BOOST_FOUND)
  set(BDSIM_ROOT_INCLUDE_PATH ${BDSIM_ROOT_INCLUDE_PATH}:${Boost_INCLUDE_DIRS})
  set(BDSIM_ROOT_INCLUDE_PATH ${BDSIM_ROOT_INCLUDE_PATH}:${Boost_INCLUDE_DIRS}/boost)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BDSVersion.hh     ${CMAKE_CURRENT_BINARY_DIR}/BDSVersion.hh     @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BDSVersionData.hh ${CMAKE_CURRENT_BINARY_DIR}/BDSVersionData.hh @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bdsim.sh.in       ${CMAKE_BINARY_DIR}/bin/bdsim.sh              @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BDSVersion.hh     DESTINATION include/bdsim COMPONENT Headers)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BDSVersionData.hh DESTINATION include/bdsim COMPONENT Headers)
install(FILES ${CMAKE_BINARY_DIR}/bin/bdsim.sh              DESTINATION bin)