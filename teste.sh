#!/bin/bash

for i in {1..16}
do
	./compiler gsamples/in/prog$i.def gsamples/out/prog$i.S
	echo "$i.S"
	cat gsamples/in/prog$i.def
	spim -file gsamples/out/prog$i.S
done
