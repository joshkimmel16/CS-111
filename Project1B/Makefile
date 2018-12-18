#define files that will be distributed
filesToDist = lab1B-client.c lab1B-server.c Makefile k.key README.md

#Default task - compile program with appropriate flags
default:
	gcc -Wall -Wextra -g -o lab1B-server lab1B-server.c -lmcrypt
	gcc -Wall -Wextra -g -o lab1B-client lab1B-client.c -lmcrypt

#Build only client program
client:
	gcc -Wall -Wextra -g -o lab1B-client lab1B-client.c -lmcrypt

#Build only server program
server:
	gcc -Wall -Wextra -g -o lab1B-server lab1B-server.c -lmcrypt

#Remove all files created by other Makefile commands
clean:
	rm -f lab1B-server lab1B-client *.txt *.tar.gz

#create distribution tarball
dist:
	tar -czf lab1B-403905121.tar.gz $(filesToDist)