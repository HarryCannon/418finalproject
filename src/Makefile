DEPS = lib/graph.h

graphtest: graphtest.cpp lib/graph.cpp game/game.cpp game/player.cpp ai/randomAI.cpp
	g++ -std=c++11 -fopenmp -o graphtest.out graphtest.cpp lib/* game/* ai/*
clean:
	rm *.out
