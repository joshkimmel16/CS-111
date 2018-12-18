#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#   8. average wait time per locking operation (ns)
#   9. length of list
#
# output:
#	lab2_list-1.png ... number of operations per second by thread count and synchronization method
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#

# general plot parameters
set terminal png
set datafile separator ","

# Number of Operations per Second By Thread Count and Synchronization Method 
set title "Number of Operation per Second By Thread Count and Synchronization Method"
set xlabel "Number of Threads"
set logscale x 24
set ylabel "Number of Operations per Second"
set logscale y 1000
set output 'lab2_list-1.png'

# grep out different sync methods
plot \
     "< grep 'list-m,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title 'Mutex' with linespoints lc rgb 'red', \
     "< grep 'list-s,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title 'Spin Lock' with linespoints lc rgb 'green'
    
# Comparing Average Time per Operation and Average Lock Wait Time By Number of Threads
set title "Comparing Average Time per Operation and Average Lock Wait Time By Number of Threads"
set xlabel "Number of Threads"
set logscale x 24
set ylabel "Average Wait Time"
set logscale y 50
set output 'lab2_list-2.png'

# grep out different sync methods
plot \
     "< grep 'list-m,' lab2_list.csv" using ($2):($7) \
	title 'Average Time per Operation' with linespoints lc rgb 'red', \
     "< grep 'list-m,' lab2_list.csv" using ($2):($8) \
	title 'Average Lock Wait Time' with linespoints lc rgb 'green'
    
set title "List-3: Protected Iterations that run without failure"
set xlabel "Number of Threads"
set logscale x 24
set ylabel "Number of Iterations"
set logscale y 50
set output 'lab2_list-3.png'
plot \
     "< grep 'list-id-none,' lab2_list.csv" using ($2):($3) \
	title 'Unprotected' with linespoints lc rgb 'red', \
     "< grep 'list-id-m,' lab2_list.csv" using ($2):($3) \
	title 'Mutex' with linespoints lc rgb 'green', \
     "< grep 'list-id-s,' lab2_list.csv" using ($2):($3) \
    title 'Spin Lock' with linespoints lc rgb 'blue'
    
set title "List-4: Operations per Second By Number of Threads - Mutex"
set xlabel "Number of Threads"
set logscale x 12
set ylabel "Operations per Second"
set logscale y 1000
set output 'lab2_list-4.png'
plot \
     "< grep 'list-m,[0-9]*,[0-9]*,1,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '1 List' with linespoints lc rgb 'red', \
     "< grep 'list-m,[0-9]*,[0-9]*,4,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '4 Lists' with linespoints lc rgb 'green', \
     "< grep 'list-m,[0-9]*,[0-9]*,8,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '8 Lists' with linespoints lc rgb 'blue', \
    "< grep 'list-m,[0-9]*,[0-9]*,16,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '16 Lists' with linespoints lc rgb 'yellow'
    
set title "List-5: Operations per Second By Number of Threads - Spin Lock"
set xlabel "Number of Threads"
set logscale x 12
set ylabel "Operations per Second"
set logscale y 1000
set output 'lab2_list-5.png'
plot \
     "< grep 'list-s,[0-9]*,[0-9]*,1,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '1 List' with linespoints lc rgb 'red', \
     "< grep 'list-s,[0-9]*,[0-9]*,4,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '4 Lists' with linespoints lc rgb 'green', \
     "< grep 'list-s,[0-9]*,[0-9]*,8,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '8 Lists' with linespoints lc rgb 'blue', \
    "< grep 'list-s,[0-9]*,[0-9]*,16,' lab2_list.csv" using ($2):(1000000000)/($7) \
	title '16 Lists' with linespoints lc rgb 'yellow'