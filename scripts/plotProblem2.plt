#!/usr/bin/gnuplot -persist
load '../scripts/colormap.pal'
set size ratio -1
set term postscript color
set pm3d map
set xlabel "i"
set ylabel "j"
set nokey

xMin = -250
yMin = -250
set xrange [-250 : 250];
set yrange [-250 : 250];

set output "numericalProblem2.eps"
set title "Numerical solution for problem 2"
splot "../bin/numericalProblem2" using ($1+xMin):($2+yMin):3 matrix

set output "analyticalProblem2.eps"
set title "Analytical solution for problem 2"
splot "../bin/analyticalProblem2" using ($1+xMin):($2+yMin):3 matrix


set output "differenceProblem2.eps"
set title "Difference between solutions for problem 2"
splot "../bin/differenceProblem2" using ($1+xMin):($2+yMin):3 matrix

set output "numericalProblem2Contour.eps"
set title "Numerical equipotentials for problem 2"
set isosamples 21, 21
set contour surface
set cntrparam levels auto 10
set style data lines
splot "../bin/numericalProblem2" using ($1+xMin):($2+yMin):3 matrix with lines nosurf lw 3

