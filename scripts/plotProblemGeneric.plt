#!/usr/bin/gnuplot -persist 
load '../scripts/colormap.pal' 
set size ratio -1 
set term postscript color 
set pm3d map 
set xlabel "i" 
 set ylabel "j" 
set nokey 
xMin = -100 
yMin = -100 
set xrange [-100:100]
set yrange [-100:100]
set output "numericalProblemGeneric.eps"
set title " Numerical Solution for Problem Generic"
splot "../bin/numericalProblem3" using ($1 +xMin):($2+yMin):3 matrix 
