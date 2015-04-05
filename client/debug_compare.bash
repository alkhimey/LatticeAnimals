#!/bin/bash

res_dir=results

mkdir results

fn1=$res_dir/algo1_$1
fn3=$res_dir/algo3_$1 

date

echo Running...
time ./bin/release/client_linux $1 --algo_id 1 -f $fn1.dump | tee $fn1.out

date

echo Sorting...
time sort $fn1.dump  > $fn1.sorted.dump

date 

echo Running...
time ./bin/release/client_linux $1 --algo_id 3 -f $fn3.dump | tee $fn3.out

date

echo Sorting...
time sort $fn3.dump  > $fn3.sorted.dump

date

echo Comparing...
time diff $fn1.sorted.dump $fn3.sorted.dump | tee $res_dir/result_$1.diff

date
echo Finished!
