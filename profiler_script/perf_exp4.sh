
##########
#### Experimento 4, comprobar efecto de la consolidacion en diferentes CPUs
#### 
##########

# para ello, lanzar las aplicaciones diferentes CPUs
# para comparar los tiempos, obtendran los datos estadisticos como media y desviacion
# estandar de los tiempos de ejecucion de cada aplicacion y los que se han obtenido previamente de control


nombre_experimento="Experimento4"
eventos="instructions,cycles,stalled-cycles-backend,page-faults,cache-misses,cache-references,LLC-load-misses,LLC-loads,LLC-stores,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,cs,migrations"


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
W1=$PARSEC_DIR" -a run -p parsec.blackscholes -c gcc-pthreads -i $dataset -n 4"

W2_n="bodytrack"
W2=$PARSEC_DIR" -a run -p parsec.bodytrack -c gcc-pthreads -i $dataset -n 4"


W3_n="fluidanimate"
W3=$PARSEC_DIR" -a run -p parsec.fluidanimate -c gcc-pthreads -i $dataset -n 4"


W4_n="freqmine"
W4=$PARSEC_DIR" -a run -p parsec.freqmine -c gcc-openmp -i $dataset -n 4"


W5_n="streamcluster"
W5=$PARSEC_DIR" -a run -p parsec.streamcluster -c gcc-pthreads -i $dataset -n 4"

# en la mascara 1 se los manda a los cores 1,2,3,4 de la primera cpu, que tiene 6
# en la segunda mascara, se ejecutan en los cores 1,2,3,4 de la segunda cpu, que tiene 6
mask1=0001E
mask2=00780

# usar perf_launcher 2 para lanzar las aplicaciones

#1blackscholes y bodytrack
echo $W1 > "./cmd1.sh"
echo $W2 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP1" $eventos 
#2fluidanimate y bodytrack
echo $W3 > "./cmd1.sh"
echo $W2 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP2" $eventos

#3blackscholes y fluidanimate
echo $W1 > "./cmd1.sh"
echo $W3 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP3" $eventos
#4freqmine y blackscholes
echo $W4 > "./cmd1.sh"
echo $W1 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP4" $eventos
#5freqmine y fluidanimate
echo $W4 > "./cmd1.sh"
echo $W3 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP5" $eventos
#6freqmine y streamcluster
echo $W4 > "./cmd1.sh"
echo $W5 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP6" $eventos

#7streamcluster y bodytrack
echo $W5 > "./cmd1.sh"
echo $W2 > "./cmd2.sh"

sh perf_launcherC.sh cmd1.sh $mask1 cmd2.sh $mask2 $nombre_experimento "EXP7" $eventos
