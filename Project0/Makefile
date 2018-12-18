#define files that will be distributed
filesToDist = lab0.c Makefile README.md *.png

#Default task - compile program with appropriate flags
default:
	gcc -Wall -Wextra -g -o lab0 lab0.c

#Remove all files created by other Makefile commands
clean:
	rm -f lab0 *.txt *.tar.gz

#create distribution tarball
dist:
	tar -czf lab0-403905121.tar.gz $(filesToDist)

#Test 1: Unrecognized argument
badArg:
	./lab0 --badInput >> testing.txt 2>&1; \
	if $$? -ne "1"; then \
	echo "Program must exit with error code 1 if there is an invalid argument." >> testResults.log; \
	fi;

#Test 2: Bad Input
badInput:
	./lab0 --input iDontExist.txt >> testing.txt 2>&1; \
	if [[ $$? -ne 2 ]]; then \
	echo "Program must exit with error code 2 if input can't be opened." >> testResults.log; \
	fi;

#Test 3: Bad Output
badOutput:
	echo "IamInput" >> goodInput1.txt; \
	touch badOutput.txt; \
	chmod u-w badOutput.txt; \
	./lab0 --input goodInput1.txt --output badOutput.txt >> testing.txt 2>&1; \
	if [[ $$? -ne 3 ]]; then \
	echo "Program must exit with error code 3 if output can't be opened." >> testResults.log; \
	fi;

#Test 4: Caught segfault
segFault:
	echo "IamInput" >> goodInput2.txt; \
	touch goodOutput1.txt; \
	./lab0 --input goodInput2.txt --output goodOutput1.txt -s -c >> testing.txt 2>&1; \
	if [[ $$? -ne 4 ]]; then \
	echo "Program must exit with error code 4 if a segfault is indicated and caught." >> testResults.log; \
	fi;

#Test 5: Success
success:
	echo "IamInput" >> goodInput3.txt; \
	touch goodOutput2.txt; \
	./lab0 --input goodInput3.txt --output goodOutput2.txt >> testing.txt 2>&1; \
	if [[ $$? -ne 0 ]]; then \
	echo "Program must exit with error code 0 on success." >> testResults.log; \
	fi;

#Run all smoke tests for program
check:
	badArg badInput badOutput segFault success