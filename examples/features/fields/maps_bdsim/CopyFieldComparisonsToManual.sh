#!/bin/bash

MANDIR=~/physics/reps/bdsim
DESDIR=$MANDIR/manual/source/dev_figures

set -x

cp original_data.pdf              $DESDIR/field_raw.pdf
cp 2dexample.png                  $DESDIR/field_raw2d.png
cp 2d_interpolated_nearest.png    $DESDIR/field_nearest2d.png
cp 2d_interpolated_linear.png     $DESDIR/field_linear2d.png
cp 2d_interpolated_linear_mag.png $DESDIR/field_linear_mag2d.png
cp 2d_interpolated_cubic.png      $DESDIR/field_cubic2d.png
cp 1d_nearest_xyz.pdf             $DESDIR/field_linear2d.png
cp 1d_linear_xyz.pdf              $DESDIR/field_linear.pdf
cp 1d_linear_mag_xyz.pdf          $DESDIR/field_linear_mag.pdf
cp 1d_cubic_xyz.pdf               $DESDIR/field_cubic.pdf
cp 1d_linear_mag_comparison.pdf   $DESDIR/field_linear_comparison.pdf
