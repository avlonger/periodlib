CC=g++
CFLAGS=-Wall -O3 -std=c++0x
LDFLAGS=
BIN_PATH=bin
SRC_PATH=src
ALGORITHMS=src/dbf.cpp src/etc.cpp src/kmp.cpp src/period.cpp

all: dirs measure experiment estimate

dirs:
	mkdir -p $(BIN_PATH)

experiment:
	$(CC) $(CFLAGS) $(ALGORITHMS) src/experiment.cpp $(LDFLAGS) -o $(BIN_PATH)/experiment

measure:
	$(CC) $(CFLAGS) $(ALGORITHMS) src/measure.cpp $(LDFLAGS) -o $(BIN_PATH)/measure

estimate:
	$(CC) $(CFLAGS) $(ALGORITHMS) src/estimate.cpp $(LDFLAGS) -o $(BIN_PATH)/estimate

clean:
	rm -rf $(BIN_PATH)
