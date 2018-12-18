#define files that will be distributed
filesToDist = lab4b.c Makefile README.md

#Default task - build list
default:
	gcc -Wall -Wextra -g -std=gnu99 -o lab4b lab4b.c -lmraa -lm

#Remove all files created by other Makefile commands
clean:
	rm -f lab4b *.tar.gz

#create distribution tarball
dist:
	tar -czf lab4b-403905121.tar.gz $(filesToDist)