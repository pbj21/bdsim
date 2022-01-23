# gmad ctests

macro(gmad_test_pass testName fileName)
  add_test(NAME gmad-${testName} COMMAND gmadExec ${fileName})
endmacro()

macro(gmad_test_fail testName fileName)
  add_test(NAME gmad-${testName} COMMAND gmadExec ${fileName})
  set_tests_properties(gmad-${testName} PROPERTIES WILL_FAIL 1)
endmacro()

macro(gmad_test_pass_expression testName fileName expression)
  add_test(NAME gmad-${testName} COMMAND gmadExec ${fileName})
  set_tests_properties(gmad-${testName} PROPERTIES PASS_REGULAR_EXPRESSION ${expression})
endmacro()


# run a specific file pattern
macro(gmad_test_duplicate objectName)
  add_test(NAME gmad-duplicate-name-${objectName} COMMAND gmadExec duplicateName-${objectName}.gmad)
  set_tests_properties(gmad-duplicate-name-${objectName} PROPERTIES PASS_REGULAR_EXPRESSION "already defined")
endmacro()