# este launcher solo se utiliza en los lanzamientos de dos aplicaciones

# recibe como argumentos los comandos a ejecutar y el nombre del experimento

# perflauncher2 cmd1 cmd2 nombreExperimento nombreSubExperimento eventos
experimento=$3
subexp=$4
eventos=$5



parser="parser.sh"
scaler="scaler"

# utiliza el script simul para lanzar dos aplicaciones y el objetivo es que se le aplique la herramienta perf a ese sciprt



# lanzar el profiler



# a simul hay que pasarle los comandos con el taskset incluido
# cada comando necesita dos cores diferentes para ejecutarse


temp_file=$(mktemp)  # raw data
times=4



# en la CPU 0
CPU=0
# se deja el core 0 vacio
mask1="00000006"
mask2="00000018"

nombre_totalTiempos="./procesado/"$experimento"/CPU_$CPU/"$subexp"_tiempos.dat"

# crear directorio para el experimento
mkdir -p "./procesado/"$experimento"/CPU_$CPU"

# si existe fichero de totaltiempos, borrarlo
if [ -f $nombre_totalTiempos ]; then
    rm $nombre_totalTiempos
fi

# loop para las iteraciones
for iter in $(seq 1 $times); do
    # preparar comando de simul que tiene que lanzar perf
    com1="taskset $mask1 sh $1"
    com2="taskset $mask2 sh $2"
    echo $com1 > "cmd1_a.sh"
    echo $com2 > "cmd2_a.sh"
    

    archivo_salidaP="./procesado/"$experimento"/CPU_$CPU/"$subexp"_"$iter".dat"  # parsed
    archivo_salidaN="./procesado/"$experimento"/CPU_$CPU/"$subexp"_"$iter".ndat" # escalado

    nombre_tiempos="temp.txt"
    # lanzar simul con perf
    comando="bash ./Simul.sh cmd1_a.sh cmd2_a.sh $nombre_tiempos"
    comandoPrincipal="perf stat -x: -I 100 -C $CPU -e $eventos -o $temp_file $comando"

    $comandoPrincipal
    
    
    # ejecutar comando principal
    # agrupar tiempos obtenidos en simul en otro fichero
    
    cat $nombre_tiempos >> $nombre_totalTiempos
    # parsear salida
    ./$parser $temp_file $archivo_salidaP 1 #el 1 es para generar ipc

    # escalar los datos
    ./$scaler $archivo_salidaP $archivo_salidaN

    rm $temp_file

done


# en la CPU 1
CPU=1
# se deja el core 0 vacio
mask1="00000180"
mask2="00000600"
mkdir -p "./procesado/"$experimento"/CPU_$CPU"
nombre_totalTiempos="./procesado/"$experimento"/CPU_$CPU/"$subexp"_tiempos.dat"

# loop para las iteraciones
for iter in $(seq 1 $times); do
    # preparar comando de simul que tiene que lanzar perf
    com1="taskset $mask1 sh $1"
    com2="taskset $mask2 sh $2"
    echo $com1 > "cmd1_a.sh"
    echo $com2 > "cmd2_a.sh"
    

    archivo_salidaP="./procesado/"$experimento"/CPU_$CPU/"$subexp"_"$iter".dat"  # parsed
    archivo_salidaN="./procesado/"$experimento"/CPU_$CPU/"$subexp"_"$iter".ndat" # escalado
    nombre_tiempos="temp.txt"
    # lanzar simul con perf
    comando="bash ./Simul.sh cmd1_a.sh cmd2_a.sh $nombre_tiempos"
    comandoPrincipal="perf stat -x: -I 100 -C $CPU -e $eventos -o $temp_file $comando"

    $comandoPrincipal
    # ejecutar comando principal
    # agrupar tiempos obtenidos en simul en otro fichero
    cat $nombre_tiempos >> $nombre_totalTiempos

    # parsear salida
    ,/$parser $temp_file $archivo_salidaP 1 #el 1 es para generar ipc

    # escalar los datos
    ,/$scaler $archivo_salidaP $archivo_salidaN

    rm $temp_file

done

rm $nombre_tiempos




