cmake_minimum_required (VERSION 3.2)
project (externalcpp)

# point cmake to my own custom installation directory that's not a system dir
# this is where BDSIMConfig.cmake exists
set(CMAKE_PREFIX_PATH /Users/nevay/physics/reps/bdsim-develop-install/lib/cmake/bdsim)

# find the package and set up variables
find_package(BDSIM REQUIRED)

# make a program and link to bdsim libraries
add_executable(customprogram customprogram.cc)
target_include_directories(customprogram PUBLIC ${BDSIM_INCLUDE_DIR})
target_link_libraries(customprogram ${BDSIM_LIBRARIES})
