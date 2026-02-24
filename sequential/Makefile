#CXXFLAGS=-I path/to/rapidjson
LDFLAGS=-lcurl -lpthread
LD=g++
CC=g++
CXXFLAGS=-std=c++17

all: level_client

level_client: level_client.o
	$(LD) $< -o $@ $(LDFLAGS)

level_client.o: level_client.cpp
	$(CC) $(CXXFLAGS) -c level_client.cpp -o level_client.o

clean:
	-rm level_client level_client.o
