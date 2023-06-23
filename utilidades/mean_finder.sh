#!/bin/sh

# Uso: ./mean_finder.sh directorio archivo_tiempos archivo_estadisticas 

# Mirar que tiene un parametro de entrada
if [ $# -ne 3 ]; then
  echo "Uso: $0 directorio archivo_tiempos archivo_estadisticas"
  exit 1
fi



# Directorio de resultados
resultados_dir=$1
# Nombre del archivo de tiempos
tiempos_archivo=$2

if [ ! -f "$tiempos_archivo" ]; then
  echo "El archivo de tiempos no existe"
  exit 1
fi
# Nombre del archivo de estadísticas (media)
estadisticas_archivo=$3

if [ ! -f "$estadisticas_archivo" ]; then
  echo "El archivo de estadisticas no existe"
  exit 1
fi

# Leer la media del archivo de estadísticas
media=$(head -n 1 "$estadisticas_archivo")

# Variables para almacenar el tiempo más cercano a la media
tiempo_cercano=""
indice_tiempo_cercano=""
indice=1
diferencia_abs_min=1000000
# Iterar sobre el archivo de tiempos
while IFS= read -r tiempo; do
  # Calcular la diferencia entre el tiempo actual y la media
  diferencia=$(echo "$tiempo - $media" | bc)
  diferencia=$(printf "%.4f" $diferencia)
  

  # Calcular el valor absoluto de la diferencia
  diferencia_abs=$(echo "scale=4; if ($diferencia < 0) -($diferencia) else $diferencia" | bc)
  diferencia_abs=$(printf "%.4f" $diferencia_abs) 
  
  # Si es la primera iteración, asignar el primer tiempo como el más cercano
  if [ -z "$tiempo_cercano" ]; then
    tiempo_cercano="$tiempo"
    indice=1
    diferencia_abs_min="$diferencia_abs"
    indice_tiempo_cercano="$indice"
    continue
  fi

  
  indice=$((indice+1))


  # comprobar que diferencia_abs sea mas pequeña que diferencia_abs_min, si lo es, print
  res=$(echo "$diferencia_abs < $diferencia_abs_min" | bc -l) 

  
  # Si la diferencia actual es menor que la diferencia anterior, actualizar los valores
  if [ $res = 1 ]; then
    tiempo_cercano="$tiempo"
    indice_tiempo_cercano="$indice"  
    diferencia_abs_min="$diferencia_abs"
  fi
  

done < "$tiempos_archivo"

# mostrando indice del tiempo mas cercano
echo "$indice_tiempo_cercano"