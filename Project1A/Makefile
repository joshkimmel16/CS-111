#define files that will be distributed
filesToDist = lab1A.c Makefile README.md

#Default task - compile program with appropriate flags
default:
	gcc -Wall -Wextra -g -o lab1A lab1A.c

#Remove all files created by other Makefile commands
clean:
	rm -f lab1A *.txt *.tar.gz

#create distribution tarball
dist:
	tar -czf lab1A-403905121.tar.gz $(filesToDist)