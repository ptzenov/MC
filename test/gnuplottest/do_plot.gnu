set style line 1 linetype 1 linewidth 1.5
set style line 2 linetype 1 linewidth 1.5
set style line 1 linetype 1 linecolor rgb '#000000'
set style line 2 linetype 1 linecolor rgb '#FF0000'
plot  'data.dat' index 0 linestyle 1 with lines,  'data.dat' index 1 linestyle 2 with linespoints