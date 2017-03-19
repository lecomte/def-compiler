#!/bin/bash

for i in {1..9}
do
	./compiler dsamples/0$i.def dsamples/0$i.out
	echo "0$i.out"
	diff -w dsamples/0$i.out dsamples/out0$i.def
done
for i in {10..17}
do
	./compiler dsamples/$i.def dsamples/$i.out
	echo "$i.out"
	diff -w dsamples/$i.out dsamples/out$i.def
done
