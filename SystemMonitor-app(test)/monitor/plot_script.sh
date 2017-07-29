#!/usr/bin/gnuplot
set terminal png
set output "CpuUsage.png"
set title "Cpu resource usage"
set xlabel "Time"
set ylabel "Frequency"
plot "CpuUsage.dat" with lines linetype 2