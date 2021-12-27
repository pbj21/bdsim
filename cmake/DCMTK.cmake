
option(USE_DICOM "Use DCMTK library for DICOM file handling" OFF)
if (USE_DICOM)
  message(STATUS "DICOM module ON")

  #find_package(DCMTK REQUIRED CONFIG)
  find_package(DCMTK REQUIRED)

  if (DCMTK_FOUND)
     message(STATUS "Found DCMTK ${DCMTK_VERSION} ${DCMKTK_INCLUDE_DIRS}")
     include_directories(SYSTEM ${DCMKTK_INCLUDE_DIRS})
     add_definitions("-DUSE_DICOM")
  else()
     message(FATAL_ERROR "USE_DICOM turned on, but no DCMTK library found.")
  endif()
endif()

