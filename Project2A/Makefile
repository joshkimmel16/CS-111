#define files that will be distributed
filesToDist = lab2_add.c lab2_list.c SortedList.h SortedList.c Makefile README.md *.png

#Default task - build both add and list
default:
	gcc -pthread -Wall -Wextra -g -o lab2_add lab2_add.c
	gcc -pthread -Wall -Wextra -g -o lab2_list SortedList.c lab2_list.c

#Build add program
add:
	gcc -pthread -Wall -Wextra -g -o lab2_add lab2_add.c

#Build list program
list:
	gcc -pthread -Wall -Wextra -g -o lab2_list SortedList.c lab2_list.c

#Generate data for add test 1
testAdd1:
	./lab2_add --threads 1 --iterations 100 >> lab2_add.csv
	./lab2_add --threads 1 --iterations 1000 >> lab2_add.csv
	./lab2_add --threads 1 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 1 --iterations 100000 >> lab2_add.csv
	./lab2_add --threads 3 --iterations 100 >> lab2_add.csv
	./lab2_add --threads 3 --iterations 1000 >> lab2_add.csv
	./lab2_add --threads 3 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 3 --iterations 100000 >> lab2_add.csv
	./lab2_add --threads 5 --iterations 100 >> lab2_add.csv
	./lab2_add --threads 5 --iterations 1000 >> lab2_add.csv
	./lab2_add --threads 5 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 5 --iterations 100000 >> lab2_add.csv

#Generate data for add test 2
testAdd2:
	./lab2_add --threads 2 --iterations 10 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 20 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 40 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 80 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 100 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 1000 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 10000 --yield >> lab2_add.csv
	./lab2_add --threads 2 --iterations 100000 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 10 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 20 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 40 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 80 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 100 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 1000 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 10000 --yield >> lab2_add.csv
	./lab2_add --threads 4 --iterations 100000 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 20 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 40 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 80 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 100 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 1000 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10000 --yield >> lab2_add.csv
	./lab2_add --threads 8 --iterations 100000 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 10 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 20 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 40 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 80 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 100 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 1000 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 10000 --yield >> lab2_add.csv
	./lab2_add --threads 12 --iterations 100000 --yield >> lab2_add.csv

#generate data for add test 3
testAdd3:
	./lab2_add --threads 2 --iterations 100 >> lab2_add.csv
	./lab2_add --threads 2 --iterations 1000 >> lab2_add.csv
	./lab2_add --threads 2 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 2 --iterations 100000 >> lab2_add.csv
	./lab2_add --threads 8 --iterations 100 >> lab2_add.csv
	./lab2_add --threads 8 --iterations 1000 >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 8 --iterations 100000 >> lab2_add.csv

#generate data for add test 4
testAdd4:
	./lab2_add --threads 2 --iterations 1000 --yield --sync m >> lab2_add.csv
	./lab2_add --threads 4 --iterations 1000 --yield --sync m >> lab2_add.csv
	./lab2_add --threads 2 --iterations 1000 --yield --sync s >> lab2_add.csv
	./lab2_add --threads 4 --iterations 1000 --yield --sync s >> lab2_add.csv
	./lab2_add --threads 8 --iterations 1000 --yield --sync s >> lab2_add.csv
	./lab2_add --threads 12 --iterations 1000 --yield --sync s >> lab2_add.csv
	./lab2_add --threads 2 --iterations 1000 --yield --sync c >> lab2_add.csv
	./lab2_add --threads 4 --iterations 1000 --yield --sync c >> lab2_add.csv
	./lab2_add --threads 8 --iterations 1000 --yield --sync c >> lab2_add.csv
	./lab2_add --threads 12 --iterations 1000 --yield --sync c >> lab2_add.csv

#generate data for add test 5
testAdd5:
	./lab2_add --threads 1 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 2 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 4 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 12 --iterations 10000 >> lab2_add.csv
	./lab2_add --threads 1 --iterations 10000 --sync m >> lab2_add.csv
	./lab2_add --threads 2 --iterations 10000 --sync m >> lab2_add.csv
	./lab2_add --threads 4 --iterations 10000 --sync m >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10000 --sync m >> lab2_add.csv
	./lab2_add --threads 12 --iterations 10000 --sync m >> lab2_add.csv
	./lab2_add --threads 1 --iterations 10000 --sync s >> lab2_add.csv
	./lab2_add --threads 2 --iterations 10000 --sync s >> lab2_add.csv
	./lab2_add --threads 4 --iterations 10000 --sync s >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10000 --sync s >> lab2_add.csv
	./lab2_add --threads 12 --iterations 10000 --sync s >> lab2_add.csv
	./lab2_add --threads 1 --iterations 10000 --sync c >> lab2_add.csv
	./lab2_add --threads 2 --iterations 10000 --sync c >> lab2_add.csv
	./lab2_add --threads 4 --iterations 10000 --sync c >> lab2_add.csv
	./lab2_add --threads 8 --iterations 10000 --sync c >> lab2_add.csv
	./lab2_add --threads 12 --iterations 10000 --sync c >> lab2_add.csv

#generate data for list test 1
testList1:
	./lab2_list --threads 2 --iterations 1 >> lab2_list.csv
	./lab2_list --threads 2 --iterations 10 >> lab2_list.csv
	./lab2_list --threads 2 --iterations 100 >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1000 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 10 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 100 >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1000 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 10 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 100 >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1000 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 10 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 100 >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1000 >> lab2_list.csv

#generate data for list test 2
testList2:
	./lab2_list --threads 2 --iterations 1 --yield i >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1 --yield d >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1 --yield il >> lab2_list.csv
	./lab2_list --threads 2 --iterations 1 --yield dl >> lab2_list.csv
	./lab2_list --threads 2 --iterations 2 --yield i >> lab2_list.csv
	./lab2_list --threads 2 --iterations 2 --yield d >> lab2_list.csv
	./lab2_list --threads 2 --iterations 2 --yield il >> lab2_list.csv
	./lab2_list --threads 2 --iterations 2 --yield dl >> lab2_list.csv
	./lab2_list --threads 2 --iterations 4 --yield i >> lab2_list.csv
	./lab2_list --threads 2 --iterations 4 --yield d >> lab2_list.csv
	./lab2_list --threads 2 --iterations 4 --yield il >> lab2_list.csv
	./lab2_list --threads 2 --iterations 4 --yield dl >> lab2_list.csv
	./lab2_list --threads 2 --iterations 8 --yield i >> lab2_list.csv
	./lab2_list --threads 2 --iterations 8 --yield d >> lab2_list.csv
	./lab2_list --threads 2 --iterations 8 --yield il >> lab2_list.csv
	./lab2_list --threads 2 --iterations 8 --yield dl >> lab2_list.csv
	./lab2_list --threads 2 --iterations 16 --yield i >> lab2_list.csv
	./lab2_list --threads 2 --iterations 16 --yield d >> lab2_list.csv
	./lab2_list --threads 2 --iterations 16 --yield il >> lab2_list.csv
	./lab2_list --threads 2 --iterations 16 --yield dl >> lab2_list.csv
	./lab2_list --threads 2 --iterations 32 --yield i >> lab2_list.csv
	./lab2_list --threads 2 --iterations 32 --yield d >> lab2_list.csv
	./lab2_list --threads 2 --iterations 32 --yield il >> lab2_list.csv
	./lab2_list --threads 2 --iterations 32 --yield dl >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1 --yield i >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1 --yield d >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1 --yield il >> lab2_list.csv
	./lab2_list --threads 4 --iterations 1 --yield dl >> lab2_list.csv
	./lab2_list --threads 4 --iterations 2 --yield i >> lab2_list.csv
	./lab2_list --threads 4 --iterations 2 --yield d >> lab2_list.csv
	./lab2_list --threads 4 --iterations 2 --yield il >> lab2_list.csv
	./lab2_list --threads 4 --iterations 2 --yield dl >> lab2_list.csv
	./lab2_list --threads 4 --iterations 4 --yield i >> lab2_list.csv
	./lab2_list --threads 4 --iterations 4 --yield d >> lab2_list.csv
	./lab2_list --threads 4 --iterations 4 --yield il >> lab2_list.csv
	./lab2_list --threads 4 --iterations 4 --yield dl >> lab2_list.csv
	./lab2_list --threads 4 --iterations 8 --yield i >> lab2_list.csv
	./lab2_list --threads 4 --iterations 8 --yield d >> lab2_list.csv
	./lab2_list --threads 4 --iterations 8 --yield il >> lab2_list.csv
	./lab2_list --threads 4 --iterations 8 --yield dl >> lab2_list.csv
	./lab2_list --threads 4 --iterations 16 --yield i >> lab2_list.csv
	./lab2_list --threads 4 --iterations 16 --yield d >> lab2_list.csv
	./lab2_list --threads 4 --iterations 16 --yield il >> lab2_list.csv
	./lab2_list --threads 4 --iterations 16 --yield dl >> lab2_list.csv
	./lab2_list --threads 4 --iterations 32 --yield i >> lab2_list.csv
	./lab2_list --threads 4 --iterations 32 --yield d >> lab2_list.csv
	./lab2_list --threads 4 --iterations 32 --yield il >> lab2_list.csv
	./lab2_list --threads 4 --iterations 32 --yield dl >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1 --yield i >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1 --yield d >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1 --yield il >> lab2_list.csv
	./lab2_list --threads 8 --iterations 1 --yield dl >> lab2_list.csv
	./lab2_list --threads 8 --iterations 2 --yield i >> lab2_list.csv
	./lab2_list --threads 8 --iterations 2 --yield d >> lab2_list.csv
	./lab2_list --threads 8 --iterations 2 --yield il >> lab2_list.csv
	./lab2_list --threads 8 --iterations 2 --yield dl >> lab2_list.csv
	./lab2_list --threads 8 --iterations 4 --yield i >> lab2_list.csv
	./lab2_list --threads 8 --iterations 4 --yield d >> lab2_list.csv
	./lab2_list --threads 8 --iterations 4 --yield il >> lab2_list.csv
	./lab2_list --threads 8 --iterations 4 --yield dl >> lab2_list.csv
	./lab2_list --threads 8 --iterations 8 --yield i >> lab2_list.csv
	./lab2_list --threads 8 --iterations 8 --yield d >> lab2_list.csv
	./lab2_list --threads 8 --iterations 8 --yield il >> lab2_list.csv
	./lab2_list --threads 8 --iterations 8 --yield dl >> lab2_list.csv
	./lab2_list --threads 8 --iterations 16 --yield i >> lab2_list.csv
	./lab2_list --threads 8 --iterations 16 --yield d >> lab2_list.csv
	./lab2_list --threads 8 --iterations 16 --yield il >> lab2_list.csv
	./lab2_list --threads 8 --iterations 16 --yield dl >> lab2_list.csv
	./lab2_list --threads 8 --iterations 32 --yield i >> lab2_list.csv
	./lab2_list --threads 8 --iterations 32 --yield d >> lab2_list.csv
	./lab2_list --threads 8 --iterations 32 --yield il >> lab2_list.csv
	./lab2_list --threads 8 --iterations 32 --yield dl >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1 --yield i >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1 --yield d >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1 --yield il >> lab2_list.csv
	./lab2_list --threads 12 --iterations 1 --yield dl >> lab2_list.csv
	./lab2_list --threads 12 --iterations 2 --yield i >> lab2_list.csv
	./lab2_list --threads 12 --iterations 2 --yield d >> lab2_list.csv
	./lab2_list --threads 12 --iterations 2 --yield il >> lab2_list.csv
	./lab2_list --threads 12 --iterations 2 --yield dl >> lab2_list.csv
	./lab2_list --threads 12 --iterations 4 --yield i >> lab2_list.csv
	./lab2_list --threads 12 --iterations 4 --yield d >> lab2_list.csv
	./lab2_list --threads 12 --iterations 4 --yield il >> lab2_list.csv
	./lab2_list --threads 12 --iterations 4 --yield dl >> lab2_list.csv
	./lab2_list --threads 12 --iterations 8 --yield i >> lab2_list.csv
	./lab2_list --threads 12 --iterations 8 --yield d >> lab2_list.csv
	./lab2_list --threads 12 --iterations 8 --yield il >> lab2_list.csv
	./lab2_list --threads 12 --iterations 8 --yield dl >> lab2_list.csv
	./lab2_list --threads 12 --iterations 16 --yield i >> lab2_list.csv
	./lab2_list --threads 12 --iterations 16 --yield d >> lab2_list.csv
	./lab2_list --threads 12 --iterations 16 --yield il >> lab2_list.csv
	./lab2_list --threads 12 --iterations 16 --yield dl >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield i >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield d >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield il >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield dl >> lab2_list.csv

#generate data for list test 3
testList3:
	./lab2_list --threads 12 --iterations 32 --yield i --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield d --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield l --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield il --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield dl --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield id --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield idl --sync m >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield i --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield d --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield l --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield il --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield dl --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield id --sync s >> lab2_list.csv
	./lab2_list --threads 12 --iterations 32 --yield idl --sync s >> lab2_list.csv

#generate data for list test 4
testList4:
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

#Remove all files created by other Makefile commands
clean:
	rm -f lab2_add lab2_list *.csv *.txt *.tar.gz

#create distribution tarball
dist:
	tar -czf lab2a-403905121.tar.gz $(filesToDist)