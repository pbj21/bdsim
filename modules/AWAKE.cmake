# awake experiment-specific classes
option(USE_AWAKE "Use AWAKE Module" OFF)
if (USE_AWAKE)
    add_definitions("-DUSE_AWAKE")
    message(STATUS "AWAKE module ON")
endif()
