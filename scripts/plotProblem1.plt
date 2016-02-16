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
set yrange [-250 : 250];
set xrange [-250 : 250];

set output "numericalProblem1.eps"
set title "Numerical solution for problem 1"
splot "../bin/numericalProblem1" using ($1+xMin):($2+yMin):3 matrix

set output "analyticalProblem1.eps"
set title "Analytical Soultion for problem 1"
splot "../bin/analyticalProblem1" using ($1+xMin):($2+yMin):3 matrix

set output "differenceProblem1.eps"
set title "Difference between analytical and numerical solutions for problem 1"
splot "../bin/differenceProblem1" using ($1+xMin):($2+yMin):3 matrix

set output "numericalFieldProblem1.eps"
set title "Numerical field for problem 1"
set parametric;
set isosamples 100,100
plot "../bin/numericalFieldProblem1" every 10:10 using 1:2:($3*40):($4*40) with vectors

set output "numericalProblem1Contour.eps"
set title "Numerical equipotentials for problem 1"
set isosamples 21, 21
set contour surface
set cntrparam levels auto 10
set style data lines
splot "../bin/numericalProblem1" using ($1+xMin):($2+yMin):3 matrix with lines nosurf lw 3
