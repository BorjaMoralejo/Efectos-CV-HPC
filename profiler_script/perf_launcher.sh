# script auxiliar que se encarga de lanzar los programas, recoger los datos y procesarlos


# comprobar que primer argumento existe
if [ $# -ne 6 ]; then
  echo "Uso: $0 nombre_parsec nombre_experimento maxiter comando eventos"
  #               $1          $2                  $3            $4      $5     
  
  exit 1
fi




parser="parser"
scaler="scaler"
maxiter=$3
eventos=$6
comando_in=$4
experimento=$2
ejecutable=$1


for CPU in $(seq 0 1); do



  if [ "$CPU" = "0" ]; then
    # CPU 0
    
    afinidad_mask="00000002"
  else
    # CPU 1
    afinidad_mask="00000080"
  fi

  comando="taskset $afinidad_mask sh $comando_in"

  # creando carpeta de experimentos
  mkdir -p "./procesado/"$experimento"/CPU_$CPU/"


  if [ "$eventos" != "n" ]; then
    archivo_tiempos="./procesado/"$experimento"/CPU_$CPU/"$ejecutable"_tiempos.dat"
    archivo_estadisticas="./procesado/"$experimento"/CPU_$CPU/stats_"$ejecutable".dat"
  else 
    archivo_estadisticas="./procesado/"$experimento"/CPU_$CPU/stats_NOEVENTS_"$ejecutable".dat"
    archivo_tiempos="./procesado/"$experimento"/CPU_$CPU/"$ejecutable"_NOEVENTS_tiempos.dat"
  fi
  rm $archivo_tiempos



  temp_file=$(mktemp)  # raw data


  for iter in $(seq 1 $maxiter); do
    echo "Iteracion $iter"
    # comprobar si eventos es n, si lo es, no mide contadores, no crear archivo de salida
    if [ "$eventos" = "n" ]; then
      
      # lanzar el profiler con -n, (null, no mide contadores)
      COM="$comando"
    else 
    # generando nombre de archivo de salida
      archivo_salidaP="./procesado/"$experimento"/CPU_$CPU/"$ejecutable"_"$iter".dat"  # parsed
      archivo_salidaN="./procesado/"$experimento"/CPU_$CPU/"$ejecutable"_"$iter".ndat" # escalado
      # lanzar el profiler
      COM="perf stat -x: -I 100 -a -e $eventos -o $temp_file $comando"
    fi
    
    
    # usando grep, quedarse con la linea que pone: real algo

    linea=$($COM | grep "real" | head -n 1) 
    echo "Iteracion $iter ejecutada, procesando datos..."   

    # quedarse con el tiempo, formato: real xmy,yyyys, separar x e y
    minutos=$(echo $linea | cut -d 'm' -f 1 | cut -d ' ' -f 2)
    segundos=$(echo $linea | cut -d 'm' -f 2 | cut -d 's' -f 1)
    segundos=$(echo $segundos | tr ',' '.')

    # convertir a segundos
    tiempo=$(echo "$minutos * 60 + $segundos" | bc -l)
    tiempo=$(printf "%.3f" "$tiempo")

    # escribir tiempo en archivo
    echo $tiempo >> $archivo_tiempos
    

    # parsear los datos (si tiene)
    # si eventos es no n, parsear
    if [ "$eventos" != "n" ]; then
    
      
      echo sh ./$parser $temp_file $archivo_salidaP 1 # la s es para generar ipc
      sh ./$parser $temp_file $archivo_salidaP 1 # la s es para generar ipc

      # escalar los datos
      ./$scaler $archivo_salidaP $archivo_salidaN
    fi

  

    # borrar archivo temporal si existe
    if [ -f "$temp_file" ]; then
      rm "$temp_file"
    fi

    
  done

  echo "DONE!"

  # crear fichero con los datos estadisticos usando estadisticas
  echo ./estadisticas $archivo_tiempos $archivo_estadisticas
  ./estadisticas $archivo_tiempos $archivo_estadisticas


  if [ "$eventos" != "n" ]; then
    # quedarse con el indice del fichero con el valor medio de las estadisticas
    indice=$(sh mean_finder.sh ./procesado/$experimento/CPU_$CPU/ $archivo_estadisticas $archivo_tiempos)

    # copiar y renombrar el fichero con los datos del mediano
    cp "./procesado/"$experimento"/CPU_$CPU/"$ejecutable"_"$indice".dat" "./procesado/"$experimento"/CPU_$CPU/mediano_"$ejecutable".dat"
    cp "./procesado/"$experimento"/CPU_$CPU/"$ejecutable"_"$indice".ndat" "./procesado/"$experimento"/CPU_$CPU/mediano_"$ejecutable".ndat"
  fi

done

