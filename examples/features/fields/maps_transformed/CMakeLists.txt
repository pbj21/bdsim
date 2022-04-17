

simple_testing(field-map-transform-1d-b-flipx-nearest  "--file=b_field_1d_flip_x_nearest.gmad" "")
simple_testing(field-map-transform-1d-b-flipx-linear   "--file=b_field_1d_flip_x_linear.gmad"  "")
simple_testing(field-map-transform-1d-b-flipx-cubic    "--file=b_field_1d_flip_x_cubic.gmad"   "")
simple_testing(field-map-transform-1d-b-flipy-nearest  "--file=b_field_1d_flip_y_nearest.gmad" "")
simple_testing(field-map-transform-1d-b-flipy-linear   "--file=b_field_1d_flip_y_linear.gmad"  "")
simple_testing(field-map-transform-1d-b-flipy-cubic    "--file=b_field_1d_flip_y_cubic.gmad"   "")
simple_testing(field-map-transform-1d-b-flipz-nearest  "--file=b_field_1d_flip_z_nearest.gmad" "")
simple_testing(field-map-transform-1d-b-flipz-linear   "--file=b_field_1d_flip_z_linear.gmad"  "")
simple_testing(field-map-transform-1d-b-flipz-cubic    "--file=b_field_1d_flip_z_cubic.gmad"   "")

simple_testing(field-map-transform-1d-b-reflectx-nearest  "--file=b_field_1d_reflect_x_nearest.gmad" "")
simple_testing(field-map-transform-1d-b-reflectx-linear   "--file=b_field_1d_reflect_x_linear.gmad"  "")
simple_testing(field-map-transform-1d-b-reflectx-cubic    "--file=b_field_1d_reflect_x_cubic.gmad"   "")
simple_testing(field-map-transform-1d-b-reflecty-nearest  "--file=b_field_1d_reflect_y_nearest.gmad" "")
simple_testing(field-map-transform-1d-b-reflecty-linear   "--file=b_field_1d_reflect_y_linear.gmad"  "")
simple_testing(field-map-transform-1d-b-reflecty-cubic    "--file=b_field_1d_reflect_y_cubic.gmad"   "")
simple_testing(field-map-transform-1d-b-reflectz-nearest  "--file=b_field_1d_reflect_z_nearest.gmad" "")
simple_testing(field-map-transform-1d-b-reflectz-linear   "--file=b_field_1d_reflect_z_linear.gmad"  "")
simple_testing(field-map-transform-1d-b-reflectz-cubic    "--file=b_field_1d_reflect_z_cubic.gmad"   "")

simple_testing(field-map-transform-2d-b-reflectx-nearest  "--file=b_field_2d_reflect_x_nearest.gmad" "")
simple_testing(field-map-transform-2d-b-reflectx-linear   "--file=b_field_2d_reflect_x_linear.gmad"  "")
simple_testing(field-map-transform-2d-b-reflectx-cubic    "--file=b_field_2d_reflect_x_cubic.gmad"   "")
simple_testing(field-map-transform-2d-b-reflecty-cubic    "--file=b_field_2d_reflect_y_cubic.gmad"   "")

simple_testing(field-map-transform-3d-b-reflectz-nearest  "--file=b_field_3d_reflect_z_nearest.gmad" "")
simple_testing(field-map-transform-3d-b-reflectz-linear   "--file=b_field_3d_reflect_z_linear.gmad"  "")
simple_testing(field-map-transform-3d-b-reflectz-cubic    "--file=b_field_3d_reflect_z_cubic.gmad"   "")

simple_testing(field-map-transform-4d-b-reflectxy-nearest   "--file=b_field_4d_reflect_xy_nearest.gmad" "")
simple_testing(field-map-transform-4d-b-reflectxy-linear    "--file=b_field_4d_reflect_xy_linear.gmad"  "")
simple_testing(field-map-transform-4d-b-reflectxy-cubic     "--file=b_field_4d_reflect_xy_cubic.gmad"   "")

# special transforms
simple_testing(field-map-transform-2d-b-reflectxydipole "--file=b_field_2d_reflectxydipole.gmad" "")
simple_testing(field-map-transform-2d-b-reflectxzdipole "--file=b_field_2d_reflectxzdipole.gmad" "")
simple_testing(field-map-transform-3d-b-reflectxzdipole "--file=b_field_3d_reflectxzdipole.gmad" "")
simple_testing(field-map-transform-3d-b-reflectyzdipole "--file=b_field_3d_reflectyzdipole.gmad" "")
simple_testing(field-map-transform-3d-b-reflectzsolenoid "--file=b_field_3d_reflect_zsolenoid.gmad" "")


interpolator_test(interpolator-1d-transform-b-flipx-nearest  "b_field_1d_flip_x_nearest.gmad")
interpolator_test(interpolator-1d-transform-b-flipx-linear   "b_field_1d_flip_x_linear.gmad")
interpolator_test(interpolator-1d-transform-b-flipx-cubic    "b_field_1d_flip_x_cubic.gmad")
interpolator_test(interpolator-1d-transform-b-flipy-nearest  "b_field_1d_flip_y_nearest.gmad")
interpolator_test(interpolator-1d-transform-b-flipy-linear   "b_field_1d_flip_y_linear.gmad")
interpolator_test(interpolator-1d-transform-b-flipy-cubic    "b_field_1d_flip_y_cubic.gmad")
interpolator_test(interpolator-1d-transform-b-flipz-nearest  "b_field_1d_flip_z_nearest.gmad")
interpolator_test(interpolator-1d-transform-b-flipz-linear   "b_field_1d_flip_z_linear.gmad")
interpolator_test(interpolator-1d-transform-b-flipz-cubic    "b_field_1d_flip_z_cubic.gmad")

interpolator_test(interpolator-1d-transform-b-reflectx-nearest  "b_field_1d_reflect_x_nearest.gmad")
interpolator_test(interpolator-1d-transform-b-reflectx-linear   "b_field_1d_reflect_x_linear.gmad")
interpolator_test(interpolator-1d-transform-b-reflectx-cubic    "b_field_1d_reflect_x_cubic.gmad")
interpolator_test(interpolator-1d-transform-b-reflecty-nearest  "b_field_1d_reflect_y_nearest.gmad")
interpolator_test(interpolator-1d-transform-b-reflecty-linear   "b_field_1d_reflect_y_linear.gmad")
interpolator_test(interpolator-1d-transform-b-reflecty-cubic    "b_field_1d_reflect_y_cubic.gmad")
interpolator_test(interpolator-1d-transform-b-reflectz-nearest  "b_field_1d_reflect_z_nearest.gmad")
interpolator_test(interpolator-1d-transform-b-reflectz-linear   "b_field_1d_reflect_z_linear.gmad")
interpolator_test(interpolator-1d-transform-b-reflectz-cubic    "b_field_1d_reflect_z_cubic.gmad")

