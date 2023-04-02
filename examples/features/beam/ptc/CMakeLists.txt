execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

simple_testing(bunch-ptc                      "--file=ptc.gmad"                     "")
simple_testing(bunch-ptc-loop                 "--file=ptc-loop.gmad"                "")
simple_testing(bunch-ptc-loop-ntimes          "--file=ptc-loop.gmad"                "")
simple_testing(bunch-ptc-nlinesSkip           "--file=ptc-nlinesSkip.gmad"          "")
simple_testing(bunch-ptc-recreate             "--file=ptc-recreate.gmad"            "")
simple_testing(bunch-ptc-recreate-loop        "--file=ptc-recreate-loop.gmad"       "")

simple_fail(bunch-ptc-loop-file-bad        "--file=ptc-loop-fail.gmad"           "")
