# Copy example directory over for testing
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

###################
### Test Errors ###
###################

# test binary without input file
add_test(gmad-no-input-file ${gmadBinary})
set_tests_properties(gmad-no-input-file PROPERTIES WILL_FAIL 1)

# test binary without two input files
add_test(gmad-two-input-files ${gmadBinary} a;b)
set_tests_properties(gmad-two-input-files PROPERTIES WILL_FAIL 1)

# test binary with non existent input file
add_test(gmad-nonexistent ${gmadBinary} nonexistent.gmad)
set_tests_properties(gmad-nonexistent PROPERTIES WILL_FAIL 1)

# try to overwrite a reserved variable
add_test(gmad-reserved-var ${gmadBinary} reservedvariable.gmad)
set_tests_properties(gmad-reserved-var PROPERTIES WILL_FAIL 1)

# try to use a reserved function
add_test(gmad-reserved-function ${gmadBinary} reservedfunction.gmad)
set_tests_properties(gmad-reserved-function PROPERTIES WILL_FAIL 1)

# try to define a reserved name
add_test(gmad-define-reserved-name ${gmadBinary} definereservedname.gmad)
set_tests_properties(gmad-define-reserved-name PROPERTIES PASS_REGULAR_EXPRESSION "already defined")

# try to define an already defined numeric variable
add_test(gmad-define-reserved-variable ${gmadBinary} definereservedvariable.gmad)
set_tests_properties(gmad-define-reserved-variable PROPERTIES PASS_REGULAR_EXPRESSION "already defined")

# try to define an already defined string variable
add_test(gmad-define-reserved-string-variable ${gmadBinary} definereservedstringvariable.gmad)
set_tests_properties(gmad-define-reserved-string-variable PROPERTIES PASS_REGULAR_EXPRESSION "already defined")

# try to define a reserved function
add_test(gmad-define-reserved-function ${gmadBinary} definereservedfunction.gmad)
set_tests_properties(gmad-define-reserved-function PROPERTIES PASS_REGULAR_EXPRESSION "already defined")

# options with no semicolon
add_test(gmad-no-semicolon ${gmadBinary} nosemicolon.gmad)
set_tests_properties(gmad-no-semicolon PROPERTIES WILL_FAIL 1)

# addition of string and number
add_test(gmad-stringnumber ${gmadBinary} stringnumber.gmad)
set_tests_properties(gmad-stringnumber PROPERTIES WILL_FAIL 1)

# string with comma
add_test(gmad-stringcomma ${gmadBinary} stringcomma.gmad)
set_tests_properties(gmad-stringcomma PROPERTIES WILL_FAIL 1)

# string with newline
add_test(gmad-stringnewline ${gmadBinary} stringnewline.gmad)
set_tests_properties(gmad-stringnewline PROPERTIES WILL_FAIL 1)

# undefined element
add_test(gmad-undefined-element ${gmadBinary} undefinedelement.gmad)
set_tests_properties(gmad-undefined-element PROPERTIES WILL_FAIL 1)

# use undefined option
add_test(gmad-undefined-option ${gmadBinary} undefinedOption.gmad)
set_tests_properties(gmad-undefined-option PROPERTIES WILL_FAIL 1)

# use undefined element with undefined option (issue #161)
add_test(gmad-undefinedelementandoption ${gmadBinary} undefinedElementAndOption.gmad)
set_tests_properties(gmad-undefinedelementandoption PROPERTIES PASS_REGULAR_EXPRESSION "not been defined")

# duplicate name
add_test(gmad-duplicate-name ${gmadBinary} duplicateName.gmad)
set_tests_properties(gmad-duplicate-name PROPERTIES WILL_FAIL 1)

# add sampler to undefined element
add_test(gmad-add-sampler-undefined-element ${gmadBinary} addsampler_undefined_element.gmad)
set_tests_properties(gmad-add-sampler-undefined-element PROPERTIES WILL_FAIL 1)

# element with no options
add_test(gmad-element-no-args ${gmadBinary} elementnoargs.gmad)
set_tests_properties(gmad-element-no-args PROPERTIES WILL_FAIL 1)

# try to overwrite an element
add_test(gmad-overwrite-element ${gmadBinary} overwriteElement.gmad)
set_tests_properties(gmad-overwrite-element PROPERTIES WILL_FAIL 1)

# try to overwrite a variable
add_test(gmad-overwrite-variable ${gmadBinary} overwriteVariable.gmad)
set_tests_properties(gmad-overwrite-variable PROPERTIES PASS_REGULAR_EXPRESSION WARNING)

# try to overwrite a string variable
add_test(gmad-overwrite-variable-string ${gmadBinary} overwriteVariableString.gmad)
set_tests_properties(gmad-overwrite-variable-string PROPERTIES PASS_REGULAR_EXPRESSION WARNING)

# try to use an undeclared variable
add_test(gmad-undeclared-variable ${gmadBinary} undeclaredVariable.gmad)
set_tests_properties(gmad-undeclared-variable PROPERTIES PASS_REGULAR_EXPRESSION "is not declared")

# include non existent file
add_test(gmad-include-nonexistent ${gmadBinary} includenonexistent.gmad)
set_tests_properties(gmad-include-nonexistent PROPERTIES WILL_FAIL 1)

# recursive inclusion
add_test(gmad-recursive-inclusion ${gmadBinary} recursiveinclusion.gmad)
set_tests_properties(gmad-recursive-inclusion PROPERTIES WILL_FAIL 1)

##########################
### Test Functionality ###
##########################

# test functions
add_test(gmad-test-functions ${gmadBinary} testfunctions.gmad)
set_tests_properties(gmad-test-functions PROPERTIES PASS_REGULAR_EXPRESSION 3.14159265)

# test stop command
add_test(gmad-stop ${gmadBinary} stop.gmad)

# test if construct
add_test(gmad-if ${gmadBinary} if.gmad)
set_tests_properties(gmad-if PROPERTIES PASS_REGULAR_EXPRESSION 2)

# test if/else construct
add_test(gmad-ifelse ${gmadBinary} ifelse.gmad)
set_tests_properties(gmad-ifelse PROPERTIES PASS_REGULAR_EXPRESSION 2)

# test special printing
add_test(gmad-print ${gmadBinary} print.gmad)

# test access attribute
add_test(gmad-test-access-attribute ${gmadBinary} accessattribute.gmad)
set_tests_properties(gmad-test-access-attribute PROPERTIES PASS_REGULAR_EXPRESSION 0.1)

# test non-existing attribute
add_test(gmad-test-missing-access-attribute ${gmadBinary} accessMissingAttribute.gmad)
set_tests_properties(gmad-test-missing-access-attribute PROPERTIES WILL_FAIL 1)

# test access element
add_test(gmad-test-access-element ${gmadBinary} accesselement.gmad)
set_tests_properties(gmad-test-access-element PROPERTIES PASS_REGULAR_EXPRESSION plane)

# test access element outside range
add_test(gmad-test-access-element-outside-range ${gmadBinary} accesselementoutsiderange.gmad)
set_tests_properties(gmad-test-access-element-outside-range PROPERTIES WILL_FAIL 1)

# test access option
add_test(gmad-test-access-option ${gmadBinary} accessoption.gmad)
set_tests_properties(gmad-test-access-option PROPERTIES PASS_REGULAR_EXPRESSION 123)

# test add sampler
add_test(gmad-test-add-sampler ${gmadBinary} addsampler.gmad)
set_tests_properties(gmad-test-add-sampler PROPERTIES PASS_REGULAR_EXPRESSION plane)

# test add all samplers
add_test(gmad-test-add-sampler-all ${gmadBinary} addsampler_all.gmad)
set_tests_properties(gmad-test-add-sampler-all PROPERTIES PASS_REGULAR_EXPRESSION plane)

# test add samplers at all quadrupoles
add_test(gmad-test-add-sampler-element-type ${gmadBinary} addsampler_element_type.gmad)
set_tests_properties(gmad-test-add-sampler-element-type PROPERTIES PASS_REGULAR_EXPRESSION plane)

add_test(gmad-test-add-sampler-partID-filter-multiple ${gmadBinary} addsampler_multiple_partID_filter.gmad)
set_tests_properties(gmad-test-add-sampler-partID-filter-multiple PROPERTIES PASS_REGULAR_EXPRESSION "samplerarticleSetID = 1")

add_test(gmad-test-add-sampler-partID-filter ${gmadBinary} addsampler_partID_filter.gmad)
set_tests_properties(gmad-test-add-sampler-partID-filter PROPERTIES PASS_REGULAR_EXPRESSION "samplerarticleSetID = 0")

add_test(gmad-test-add-sampler-partID-filter-all ${gmadBinary} addsampler_all_partID_filter.gmad)
set_tests_properties(gmad-test-add-sampler-partID-filter-all PROPERTIES PASS_REGULAR_EXPRESSION "samplerarticleSetID = 0")

# test inheritance declaration
add_test(gmad-inherit ${gmadBinary} inherit.gmad)
set_tests_properties(gmad-inherit PROPERTIES PASS_REGULAR_EXPRESSION 0.05)

# test extension
add_test(gmad-extend ${gmadBinary} extend.gmad)
set_tests_properties(gmad-extend PROPERTIES PASS_REGULAR_EXPRESSION 0.05)

# test extending a multipole with list syntax
add_test(gmad-extend-list ${gmadBinary} extendlist.gmad)
set_tests_properties(gmad-extend-list PROPERTIES PASS_REGULAR_EXPRESION 1234)

# test non valid extension
add_test(gmad-extend-nonvalid ${gmadBinary} extendnonvalid.gmad)
set_tests_properties(gmad-extend-nonvalid PROPERTIES WILL_FAIL 1)

# test units
add_test(gmad-units ${gmadBinary} units.gmad)

# test objects
add_test(gmad-aperture                  ${gmadBinary} aperture.gmad)
add_test(gmad-atom                      ${gmadBinary} atom.gmad)
add_test(gmad-blm                       ${gmadBinary} blm.gmad)
add_test(gmad-cavitymodel               ${gmadBinary} cavitymodel.gmad)
add_test(gmad-colour                    ${gmadBinary} colour.gmad)
add_test(gmad-coolingchannel            ${gmadBinary} coolingchannel.gmad)
add_test(gmad-crystal                   ${gmadBinary} crystal.gmad)
add_test(gmad-field                     ${gmadBinary} field.gmad)
add_test(gmad-material                  ${gmadBinary} material.gmad)
add_test(gmad-physics-biasing           ${gmadBinary} physicsbiasing.gmad)
add_test(gmad-placement                 ${gmadBinary} placement.gmad)
add_test(gmad-placement-sequence        ${gmadBinary} placement_sequence.gmad)
add_test(gmad-region                    ${gmadBinary} region.gmad)
add_test(gmad-samplerplacement          ${gmadBinary} samplerplacement.gmad)
add_test(gmad-scorer                    ${gmadBinary} scorer.gmad)
add_test(gmad-scorermesh                ${gmadBinary} scorermesh.gmad)
add_test(gmad-tunnel                    ${gmadBinary} tunnel.gmad)

# test object extensions
add_test(gmad-extend-aperture         ${gmadBinary} extendaperture.gmad)
add_test(gmad-extend-atom             ${gmadBinary} extendatom.gmad)
add_test(gmad-extend-blm              ${gmadBinary} extendblm.gmad)
add_test(gmad-extend-cavitymodel      ${gmadBinary} extendcavitymodel.gmad)
add_test(gmad-extend-colour           ${gmadBinary} extendcolour.gmad)
add_test(gmad-extend-coolingchannel   ${gmadBinary} extendcoolingchannel.gmad)
add_test(gmad-extend-crystal          ${gmadBinary} extendcrystal.gmad)
add_test(gmad-extend-field            ${gmadBinary} extendfield.gmad)
add_test(gmad-extend-material         ${gmadBinary} extendmaterial.gmad)
add_test(gmad-extend-physics-biasing  ${gmadBinary} extendphysicsbiasing.gmad)
add_test(gmad-extend-placement        ${gmadBinary} extendplacement.gmad)
add_test(gmad-extend-region           ${gmadBinary} extendregion.gmad)
add_test(gmad-extend-samplerplacement ${gmadBinary} extendsamplerplacement.gmad)
add_test(gmad-extend-scorer           ${gmadBinary} extendscorer.gmad)
add_test(gmad-extend-scorermesh       ${gmadBinary} extendscorermesh.gmad)
add_test(gmad-extend-tunnel           ${gmadBinary} extendtunnel.gmad)

# test string (issue #126)
add_test(gmad-string ${gmadBinary} string.gmad)

# test arrays
add_test(gmad-array ${gmadBinary} array.gmad)
set_tests_properties(gmad-array PROPERTIES PASS_REGULAR_EXPRESSION 32)
add_test(gmad-arraystring ${gmadBinary} arraystring.gmad)

# test beam structure
add_test(gmad-beam               ${gmadBinary} beam.gmad)

# test query
add_test(gmad-query ${gmadBinary} query.gmad)
