OBJ = Source/PPFSBase.o Source/PPFSScanner.o Source/PPFSSynchronizer.o
MAIN = Source/PPFSMain.o
TESTMAIN = Source/PPFSTestMain.o
all: PPFileSync
PPFileSync: $(OBJ) $(MAIN)
	gcc $(OBJ) $(MAIN) -lssl -lcrypto -o PPFileSync
Test: $(OBJ) $(TESTMAIN)
	gcc $(OBJ) $(TESTMAIN) -lssl -lcrypto -o Test
$(OBJ): Source/PPFSBase.h Source/PPFSScanner.h Source/PPFSSynchronizer.h Source/PPFSPrerequisites.h
.PHONY: clean
clean:
	rm -f *.o

