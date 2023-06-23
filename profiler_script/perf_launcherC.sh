# este launcher solo se utiliza en los lanzamientos de dos aplicaciones

# recibe como argumentos los comandos a ejecutar y el nombre del experimento

# perflauncher2 cmd1 mask1 cmd2 mask2 nombreExperimento nombreSubExperimento eventos

cmd1=$1
mask1=$2
cmd2=$3
mask2=$4
experimento=$5
subexp=$6
eventos=$7



parser="./parser"
scaler="./scaler"

# utiliza el script simul para lanzar dos aplicaciones y el objetivo es que se le aplique la herramienta perf a ese sciprt
# a simul hay que pasarle los comandos con el taskset incluido
com1="taskset $mask1 sh $cmd1"
com2="taskset $mask2 sh $cmd2"
echo $com1 > "cmd1_a.sh"
echo $com2 > "cmd2_a.sh"
    

temp_file=$(mktemp)  # raw data
times=4



nombre_totalTiempos="./procesado/"$experimento"/"$subexp"_tiempos.dat"

# crear directorio para el experimento
mkdir -p "./procesado/"$experimento"/"

# si existe fichero de totaltiempos, borrarlo
if [ -f $nombre_totalTiempos ]; then
    rm $nombre_totalTiempos
fi

# loop para las iteraciones
for iter in $(seq 1 $times); do
    # preparar comando de simul que tiene que lanzar perf
    

    archivo_salidaP="./procesado/"$experimento"/"$subexp"_"$iter".dat"  # parsed
    archivo_salidaN="./procesado/"$experimento"/"$subexp"_"$iter".ndat" # escalado
    nombre_tiempos="temp.txt"
    # lanzar simul con perf
    comando="bash ./Simul.sh cmd1_a.sh cmd2_a.sh $nombre_tiempos"
    comandoPrincipal="perf stat -x: -I 100 -a -e $eventos -o $temp_file $comando"

    # ejecutar comando principal
    $comandoPrincipal
    
  
    # agrupar tiempos obtenidos en simul en otro fichero    
    cat $nombre_tiempos >> $nombre_totalTiempos

    # parsear salida
    $parser $temp_file $archivo_salidaP 1 #el 1 es para generar ipc

    # escalar los datos
    $scaler $archivo_salidaP $archivo_salidaN

    rm $temp_file


done
rm "cmd1_a.sh"
rm "cmd2_a.sh"
