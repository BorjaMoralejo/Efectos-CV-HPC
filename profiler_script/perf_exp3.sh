
##########
#### Experimento 3, comprobar efecto de la consolidacion
#### 
##########

# para ello, lanzar las aplicaciones en la misma cpu a la vez y comparar los tiempos
# para comparar los tiempos, obtendran los datos estadisticos como media y desviacion
# estandar de los tiempos de ejecucion de cada aplicacion y los que se han obtenido previamente de control


nombre_experimento="Experimento3"
eventos="instructions,cycles,bus-cycles,page-faults,cache-misses,cache-references,LLC-load-misses,LLC-loads,LLC-stores,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,cs,migrations"



#### PARSEC 

PARSEC_DIR="/home/borja/Downloads/parsec-benchmark-master/bin/parsecmgmt"


## Simulacion o no
simulacion="y"

if [ $simulacion = "y" ]; then
    dataset="simlarge"
else
    dataset="native"
fi



### Aplicaciones que se van a ejecutar
W1_n="blackscholes"
W1=$PARSEC_DIR" -a run -p parsec.blackscholes -c gcc-pthreads -i $dataset -n 2"

W2_n="bodytrack"
W2=$PARSEC_DIR" -a run -p parsec.bodytrack -c gcc-pthreads -i $dataset -n 2"


W3_n="fluidanimate"
W3=$PARSEC_DIR" -a run -p parsec.fluidanimate -c gcc-pthreads -i $dataset -n 2"


W4_n="freqmine"
W4=$PARSEC_DIR" -a run -p parsec.freqmine -c gcc-openmp -i $dataset -n 2"


W5_n="streamcluster"
W5=$PARSEC_DIR" -a run -p parsec.streamcluster -c gcc-pthreads -i $dataset -n 2"




# usar perf_launcher 2 para lanzar las aplicaciones

#1blackscholes y bodytrack
echo $W1 > "./cmd1.sh"
echo $W2 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP1" $eventos 
#2fluidanimate y bodytrack
echo $W3 > "./cmd1.sh"
echo $W2 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP2" $eventos

#3blackscholes y fluidanimate
echo $W1 > "./cmd1.sh"
echo $W3 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP3" $eventos
#4freqmine y blackscholes
echo $W4 > "./cmd1.sh"
echo $W1 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP4" $eventos
#5freqmine y fluidanimate
echo $W4 > "./cmd1.sh"
echo $W3 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP5" $eventos
#6freqmine y streamcluster
echo $W4 > "./cmd1.sh"
echo $W5 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP6" $eventos

#7streamcluster y bodytrack
echo $W5 > "./cmd1.sh"
echo $W2 > "./cmd2.sh"

sh perf_launcher2.sh cmd1.sh cmd2.sh $nombre_experimento "EXP7" $eventos
