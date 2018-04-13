#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "jeu_200_75_5_5.eps"
set key right bottom
set xlabel "Iteracion"
set ylabel "Beneficio"
plot 'jeu_200_75_5_5.txt' using 1 t "CurrentFirst" w l, 'jeu_200_75_5_5.txt' using 2 t "BestFirst" w l, 'jeu_200_75_5_5.txt' using 3 t "CurrentBest" w l, 'jeu_200_75_5_5.txt' using 4 t "BestBest" w l
_end_
