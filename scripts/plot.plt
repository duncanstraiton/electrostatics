#!/usr/bin/gnuplot -persist
set size ratio -1
set term postscript color
set pm3d map
set output "plot.eps"
set xlabel "i"
set ylabel "j"
set title "Electric potential due to left plate @5V, right @-5V and centre circle radius 25 @0V"
set xrange [-50: 50];
set yrange [-50: 50];
splot "../bin/electrostatics.out"
