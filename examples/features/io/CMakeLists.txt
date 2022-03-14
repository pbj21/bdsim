# rootevent output
add_subdirectory(1_rootevent)

simple_testing(io-none "--file=sm.gmad --output=none" "")
simple_testing(io-store-trajectories     "--file=1_storeTrajectories.gmad"             "")


# checks - tests that should fail

# invalid sampler name - conflicts with protected names in output structures
simple_fail(io-invalid-sampler-name "--file=invalid_sampler_name.gmad")

# trying to open outfile file in invalid location
set(TESTING_ARGS --outfile=a/fictional/path/to/data)
simple_fail(io-invalid-file-path  "--file=sm.gmad")

# unknown output format
set(TESTING_ARGS --output=abcd)
simple_fail(io-unknown-output-format "--file=sm.gmad")
