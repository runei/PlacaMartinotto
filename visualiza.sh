# !/bin/bash

cd saidas
arq=`ls`

for a in $arq
do	
 	 echo "set samples 25" > entrada.gnu
  	 echo "set isosamples 20" >> entrada.gnu
  	 echo "set pm3d map" >> entrada.gnu
  	 echo "set palette" >> entrada.gnu
 	 echo "splot '"$a"' matrix" >> entrada.gnu
 	 echo "pause 1" >> entrada.gnu
	 gnuplot < entrada.gnu
done
rm entrada.gnu
