# Required environmental variables
# NAME             EXAMPLE
#
# BDS_SITE          rhul-htcondor
# BDS_BUILD_NAME    CC7-GCC11-Geant4.11.03-develop
# BDS_TEST_TYPE     Nightly
# BDS_BUILD_TYPE    Release
# BDS_EXCLUDE_LABEL NONE

# echo all variables
message(STATUS "Site          : $ENV{BDS_SITE}")
message(STATUS "Build name    : $ENV{BDS_BUILD_NAME}")
message(STATUS "Src dir       : $ENV{BDS_SRC_DIR}")
message(STATUS "Build dir     : $ENV{BDS_BUILD_DIR}")
message(STATUS "Test type     : $ENV{BDS_TEST_TYPE}")
message(STATUS "Build type    : $ENV{BDS_BUILD_TYPE}")
message(STATUS "Exclude label : $ENV{BDS_EXCLUDE_LABEL}")

# Necessary edits:
set(CTEST_SITE       "$ENV{BDS_SITE}")
set(CTEST_BUILD_NAME "$ENV{BDS_BUILD_NAME}")

# Appropriate source for this build should be checked out already into this directory:
set(CTEST_SOURCE_DIRECTORY "$ENV{BDS_SRC_DIR}")

# Compilation will be done in this directory:
set(CTEST_BINARY_DIRECTORY "$ENV{BDS_BUILD_DIR}")

# Optional edits:
ctest_start("$ENV{BDS_TEST_TYPE}")
set(cfg_options
 -DCMAKE_BUILD_TYPE=$ENV{BDS_BUILD_TYPE}
 -DUSE_HEPMC3=ON
 -DBDSIM_BUILD_TEST_PROGRAMS=ON
)

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_UPDATE_COMMAND "git")

# this only applies to testing - not compilation
#set(CTEST_PARALLEL_LEVEL 8)

ctest_update()
ctest_configure(OPTIONS "${cfg_options}")
ctest_build()
ctest_submit()
ctest_test(EXCLUDE_LABEL "$ENV{BDS_EXCLUDE_LABEL}" EXCLUDE doxygen)
ctest_submit()
