#define files that will be distributed
filesToDist = lab2_list.c SortedList.h SortedList.c Makefile README.md *.png

#Default task - build list
default:
	gcc -pthread -lprofiler -Wall -Wextra -g -o lab2_list SortedList.c lab2_list.c

#Build list program
list:
	gcc -pthread -Wall -Wextra -g -o lab2_list SortedList.c lab2_list.c

#generate data for list test 1
testList1:
	./lab2_list --threads 1 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 16 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 24 --iterations 1000 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --sync s >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --sync s >> lab2_list.csv
	./lab2_list --threads 16 --iterations 1000 --sync s >> lab2_list.csv
	./lab2_list --threads 24 --iterations 1000 --sync s >> lab2_list.csv

#generate data for list test 2
testList2:
	./lab2_list --threads 1 --iterations 1 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 1 --iterations 2 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 1 --iterations 4 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 1 --iterations 8 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 1 --iterations 16 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 2 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 4 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 8 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 16 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 2 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 4 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 8 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 16 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 2 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 4 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 8 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 16 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 16 --iterations 1 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 16 --iterations 2 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 16 --iterations 4 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 16 --iterations 8 --yield id --lists 4 >> lab2_list.csv
	./lab2_list --threads 16 --iterations 16 --yield id --lists 4 >> lab2_list.csv

#generate data for list test 3
testList3:
	./lab2_list --threads 1 --iterations 10 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 20 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 40 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 80 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 10 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 20 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 40 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 80 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 10 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 20 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 40 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 80 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 10 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 20 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 40 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 80 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 16 --iterations 10 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 16 --iterations 20 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 16 --iterations 40 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 16 --iterations 80 --yield id --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 10 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 1 --iterations 20 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 1 --iterations 40 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 1 --iterations 80 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 10 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 20 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 40 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 80 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 10 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 20 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 40 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 80 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 10 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 20 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 40 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 80 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 16 --iterations 10 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 16 --iterations 20 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 16 --iterations 40 --yield id --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 16 --iterations 80 --yield id --lists 4 --sync s >> lab2_list.csv

#generate data for list test 4
testList4:
	./lab2_list --threads 1 --iterations 1000 --lists 1 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 8 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 16 --sync m >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 1 --sync m >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 8 --sync m >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 16 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 1 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 8 --sync m >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 16 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 1 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 8 --sync m >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 16 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 1 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 4 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 8 --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 16 --sync m >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 1 --sync s >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 8 --sync s >> lab2_list.csv
	./lab2_list --threads 1 --iterations 1000 --lists 16 --sync s >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 1 --sync s >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 8 --sync s >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 --lists 16 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 1 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 8 --sync s >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 --lists 16 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 1 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 8 --sync s >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 --lists 16 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 1 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 4 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 8 --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 --lists 16 --sync s >> lab2_list.csv


#run profiler for spin-lock
profile:
	rm -f raw.gperf
	LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads 12 --iterations 1000 --sync s
	google-pprof --text ./lab2_list ./raw.gperf > profile.out
	google-pprof --list=ExecuteThread ./lab2_list ./raw.gperf >> profile.out
	rm -f raw.gperf

#Remove all files created by other Makefile commands
clean:
	rm -f lab2_list *.csv *.txt *.tar.gz

#create distribution tarball
dist:
	tar -czf lab2b-403905121.tar.gz $(filesToDist)