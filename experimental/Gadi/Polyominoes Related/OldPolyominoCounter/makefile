PolyominoCounter: redelmeierpolyominos.cpp redelmeieralgorithm.h redelmeiergraph.h bigint.h bigint.h redelmeieralgorithm.o redelmeiergraph.o bigint.o
	g++ -o3 redelmeierpolyominos.cpp redelmeieralgorithm.o redelmeiergraph.o bigint.o -o PolyominoCounter 
redelmeieralgorithm.o: redelmeieralgorithm.cpp redelmeieralgorithm.h redelmeiergraph.h bigint.h bigint.h
	g++ -c redelmeieralgorithm.cpp 
redelmeiergraph.o: redelmeiergraph.cpp redelmeiergraph.h bigint.h
	g++ -c redelmeiergraph.cpp 
bigint.o: bigint.cpp bigint.h
	g++ -c bigint.cpp 
PolyominoMerger: polyominoMerger.cpp bigint.h bigint.o
	g++ -o PolyominoMerger polyominoMerger.cpp bigint.o 
