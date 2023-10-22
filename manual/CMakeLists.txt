# CMake for Sphinx based on
# http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/

# see bdsim/cmake/CompilerBuildFlags.cmake for USE_GOOGLE_SPHINX option

find_package(Sphinx)
# Find our theme
include(FindPythonModule)
find_python_module(sphinx_rtd_theme)

# configured documentation tools and intermediate build results
set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/doctrees")

# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")
# LATEX output directory
set(SPHINX_LATEX_DIR "${CMAKE_CURRENT_BINARY_DIR}/latex")
# Add directory to be able to add manual-pdf command in one go (hack)
file(MAKE_DIRECTORY ${SPHINX_LATEX_DIR})

configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/source/conf.py.in"
      "${BINARY_BUILD_DIR}/conf.py"
      @ONLY)

configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/source/licence.rst.in"
      "${BINARY_BUILD_DIR}/source/licence.rst"
      @ONLY)

# Helper target, not meant to be executed standalone
add_custom_target(manual-copy-files
    # Copy sources to build directory
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/source ${CMAKE_CURRENT_BINARY_DIR}/source
    # Make directory for examples
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BINARY_BUILD_DIR}/source/examples
    # Copy .rst and .png files from examples
    COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/../examples \( -iname *.rst -o -iname *.png -o -iname *.pdf \) -exec cp {} ${BINARY_BUILD_DIR}/source/examples/. \;
    COMMENT "Copying manual files to build directory"
    VERBATIM
  )

# Make HTML manual
add_custom_target(manual
      ${SPHINX_EXECUTABLE}
          -q -b html
          -c "${BINARY_BUILD_DIR}"
          -d "${SPHINX_CACHE_DIR}"
          "${CMAKE_CURRENT_BINARY_DIR}/source"
          "${SPHINX_HTML_DIR}"
      DEPENDS manual-copy-files
      COMMENT "Building HTML documentation with Sphinx"
  )

# Make PDF manual
# The number of LaTeX runs is quite conservative, but I don't expect it
# to get run often, so the little extra time won't hurt.
add_custom_target(manual-pdf
      ${SPHINX_EXECUTABLE}
          -q -b latex
          -c "${BINARY_BUILD_DIR}"
          -d "${SPHINX_CACHE_DIR}"
          "${CMAKE_CURRENT_BINARY_DIR}/source"
          "${SPHINX_LATEX_DIR}"
      WORKING_DIRECTORY ${SPHINX_LATEX_DIR} 
      COMMAND pdflatex BDSIM
      COMMAND pdflatex BDSIM
      COMMAND makeindex -s python.ist BDSIM.idx
      COMMAND pdflatex BDSIM
      COMMAND pdflatex BDSIM 
      DEPENDS manual-copy-files
      COMMENT "Building LaTeX documentation with Sphinx"
    )


if(SPHINX_FOUND AND PY_SPHINX_RTD_THEME)
    # add building to ctest
    add_test(Manual make manual)
    add_test(Manual-pdf make manual-pdf)
    set_tests_properties(Manual-pdf PROPERTIES LABELS LONG)
endif()
