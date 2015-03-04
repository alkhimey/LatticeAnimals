./bin/debug/client_linux $1 --algo_id 1 -f r1.res1
./bin/debug/client_linux $1 --algo_id 3 -f r2.res1

cat r1.res1 | while read line; do echo $line | sed 's/ /\n/g' | sort | gawk '{line=line " " $0} END {print line}' ; done > r1.res2


cat r2.res1 | while read line; do echo $line | sed 's/ /\n/g' | sort | gawk '{line=line " " $0} END {print line}' ; done > r2.res2

sort r1.res2 > r1.res3
sort r2.res2 > r2.res3

diff r1.res3 r2.res3

rm r1.res*
rm r2.res*
