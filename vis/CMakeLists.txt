# Copy files to build directory
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

bdsim_install_vis(bdsim_default_dawnfile.mac bdsim_default_gui.mac bdsim_default_icons.mac run.png bdsim_default_vis.mac bdsim_default_vis_11p2.mac)
