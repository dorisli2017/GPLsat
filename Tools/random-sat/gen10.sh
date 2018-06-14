#!/bin/bash

classname=$1
shift
varc=$1
shift

dirn=`echo rnd-$classname | tr "." "p"`

mkdir -p $dirn

cls=`bc <<< "$varc * $4" | cut -d"." -f 1`
vars=`printf "%.3d" $varc`
echo $varc $cls $vars
for seed in `seq 1 5`
do
    java -jar gen.jar $seed $varc $cls $1 $2 $3 > $dirn/fla-$classname-$vars-$seed.cnf
done
