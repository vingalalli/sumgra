INCLUDES= -I include/
CC=g++ -std=c++11
CFLAGS=-c -O3

all: Sumgra.o FileManager.o IndexManager.o SubgraphMatcher.o Trie.o MaxMatch.o
		$(CC) -O3 -o sumgra Sumgra.o FileManager.o IndexManager.o SubgraphMatcher.o Trie.o MaxMatch.o

Sumgra.o: Sumgra.cpp
	$(CC) $(CFLAGS) $(INCLUDES) Sumgra.cpp

FileManager.o: src/FileManager.cpp include/FileManager.h
	$(CC) $(CFLAGS) $(INCLUDES) src/FileManager.cpp

IndexManager.o: src/IndexManager.cpp include/IndexManager.h 
	$(CC) $(CFLAGS) $(INCLUDES) src/IndexManager.cpp

SubgraphMatcher.o: src/SubgraphMatcher.cpp include/SubgraphMatcher.h
	$(CC) $(CFLAGS) $(INCLUDES) src/SubgraphMatcher.cpp
	
Trie.o: src/Trie.cpp include/Trie.h
	$(CC) $(CFLAGS) $(INCLUDES) src/Trie.cpp	
	
MaxMatch.o: src/MaxMatch.cpp include/MaxMatch.h
	$(CC) $(CFLAGS) $(INCLUDES) src/MaxMatch.cpp	

clean:
	rm sumgra
	rm -f *.o
