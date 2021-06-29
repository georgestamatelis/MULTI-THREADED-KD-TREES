all:neigh class time-Benchmark odysMAIN

class:KDTree.cpp Classification.cpp
	g++ -pthread -o class -g3 Classification.cpp KDTree.cpp
neigh:KDTree.cpp Neighbors.cpp
	g++ -pthread -o neigh -g3 Neighbors.cpp KDTree.cpp
time-Benchmark:time-Benchmark.cpp KDTree.cpp
	g++ -pthread -o time-Benchmark -g3 time-Benchmark.cpp KDTree.cpp
odysMAIN:KDTree.cpp odysMAIN.cpp
	g++ -pthread -o odysMAIN -g3 odysMAIN.cpp KDTree.cpp
clean:
	rm neigh class time-Benchmark odysMAIN