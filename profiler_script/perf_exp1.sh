
##########
#### Experimento 1, comprobar efecto del profiler en el tiempo de ejecución
#### de las aplicaciones
##########

# para ello, lanzar las aplicaciones con y sin profiler y comparar los tiempos
# para comparar los tiempos, obtendran los datos estadisticos como media y desviacion
# estandar de los tiempos de ejecucion de cada aplicacion


nombre_experimento="Experimento1"
eventos="instructions,cycles,stalled-cycles-backend,stalled-cycles-frontend,minor-faults,major-faults,LLC-load-misses,LLC-loads,LLC-stores,LLC-store-misses,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-dcache-store-misses,cs,migrations"
times=4
mask=0000001


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



mkdir -p logs/$nombre_experimento


# preparando fichero para que lo utilice el sciprt auxiliar
shell="tempShell.sh"




## BLACKSCHOLES
        echo $W1 > $shell

        # lanzando experimento1.a1
      #  echo "lanzando experimento1.a1"
       # sh ./perf_launcher.sh $W1_n $nombre_experimento $mask $times $shell $eventos > logs/$nombre_experimento/$W1_n"_perf"
        #echo "lanzando experimento1.a2"
        #sh ./perf_launcher.sh $W1_n $nombre_experimento $mask $times $shell n > logs/$nombre_experimento/$W1_n"_noperf"

# para de ejecutar
exit 0
## BODYTRACK
        echo $W2 > $shell

        # lanzando experimento1.b
        echo "lanzando experimento1.b1"
        sh ./perf_launcher.sh $W2_n $nombre_experimento $times $shell $eventos > logs/$nombre_experimento/$W2_n"_perf"
        echo "lanzando experimento1.b2"
        sh ./perf_launcher.sh $W2_n $nombre_experimento $times $shell n > logs/$nombre_experimento/$W2_n"_noperf"
        

## FLUIDANIMATE
        echo $W3 > $shell

        # lanzando experimento1.c
        echo "lanzando experimento1.c1"
        sh ./perf_launcher.sh $W3_n $nombre_experimento $times $shell $eventos > logs/$nombre_experimento/$W3_n"_perf"
        echo "lanzando experimento1.c2"
        sh ./perf_launcher.sh $W3_n $nombre_experimento $times $shell n > logs/$nombre_experimento/$W3_n"_noperf"



## FREQMINE         
        echo $W4 > $shell

        # lanzando experimento1.d
        echo "lanzando experimento1.d1"
        sh ./perf_launcher.sh $W4_n $nombre_experimento $times $shell $eventos > logs/$nombre_experimento/$W4_n"_perf"
        echo "lanzando experimento1.d2"
        sh ./perf_launcher.sh $W4_n $nombre_experimento $times $shell n > logs/$nombre_experimento/$W4_n"_noperf" 


## STREAMCLUSTER
        echo $W5 > $shell

        # lanzando experimento1.e
        echo "lanzando experimento1.e1"
        sh ./perf_launcher.sh $W5_n $nombre_experimento $times $shell $eventos > logs/$nombre_experimento/$W5_n"_perf"
        echo "lanzando experimento1.e2"
        sh ./perf_launcher.sh $W5_n $nombre_experimento $times $shell n > logs/$nombre_experimento/$W5_n"_noperf"



rm $shell








