#!/usr/bin/gnuplot -persist
set size ratio -1
set term postscript color
set pm3d map
set output "numericalProblem1.eps"
set xlabel "i"
set ylabel "j"
set title "Numerical solution for problem 1"
set xrange [-50 : 50];
set yrange [-50 : 50];
splot "../bin/numericalProblem1"

set size ratio -1
set term postscript color
set pm3d map
set output "analyticalProblem1.eps"
set xlabel "i"
set ylabel "j"
set title "Analytical Soultion for problem 1"
set xrange [-50 : 50];
set yrange [-50 : 50];
splot "../bin/analyticalProblem1"

set size ratio -1
set term postscript color
set pm3d map
set output "differenceProblem1.eps"
set xlabel "i"
set ylabel "j"
set title "Difference between analytical and numerical solutions for problem 1"
set xrange [-50 : 50];
set yrange [-50 : 50];
splot "../bin/differenceProblem1"
