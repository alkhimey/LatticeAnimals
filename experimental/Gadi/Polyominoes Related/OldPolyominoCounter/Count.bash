#!/bin/bash

if [[ $# -ne 5 ]]; then
	echo "usage: $0 [d] [n] [breakPoint] [numberOfChunks] [startingFrom]"
else
	for ((i=$5; i <= $4; i++)); do
		name="results/ProperPolyominoCount-$1d-$2n-breakAt$3-$iof$4.txt"
		if [[ ! -f $name ]]; then ./PolyominoCounter $1 $2 $3 $4 $i; fi
	done
	./PolyominoMerger $1 $2 $3 $4
fi
