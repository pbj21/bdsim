if (USE_GDML)
  simple_testing(gdml-colours-old        "--file=14a_gdml_colours_old.gmad --output=none"       ${OVERLAP_CHECK})
  simple_testing(gdml-colours-materials  "--file=14b_gdml_material_colours.gmad --output=none"  ${OVERLAP_CHECK})
endif()