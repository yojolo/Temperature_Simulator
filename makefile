all: master slave coordinator
	mpirun -np 1 ./master

master: master.cpp
	mpic++ master.cpp -o master

slave: slave.cpp CSlave.Cpp
	mpic++ slave.cpp CSlave.cpp -o slave

coordinator: coordinator.cpp CCoordinator.cpp CTerminal.cpp
	mpic++ coordinator.cpp CCoordinator.cpp CTerminal.cpp -o coordinator -lSDL2

.PHONY: clean 

clean: 
	rm -f master slave coordinator