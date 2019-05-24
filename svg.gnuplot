set terminal svg noenhanced size 750,550 dynamic fontscale 0.8

set key top horizontal
set auto x
set ylabel "time [ms]\n⟵ lower is better"
set yrange [0:500]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtic rotate by -45 scale 0

set output "utf-32.svg"
set title "UTF-32 ".t
plot for [i=2:*] 'utf-32.dat' using i:xtic(1) ti col

set output "utf-16.svg"
set title "UTF-16 ".t
plot for [i=2:*] 'utf-16.dat' using i:xtic(1) ti col
