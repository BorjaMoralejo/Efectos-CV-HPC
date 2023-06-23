#!/bin/bash
#export LC_NUMERIC="en_US.UTF-8"


# Verifica que se hayan proporcionado los nombres de los dos programas y el nombre del archivo de salida
if [ $# -ne 3 ]; then
  echo "Uso: $0 programa1 programa2 archivo_salida"
  exit 1
fi



# funcion que escribe sampletext
function obtener_tiempo {

    linea=$(cat $1 | grep "real" | head -n 1) 

    # quedarse con el tiempo, formato: real xmy,yyyys, separar x e y
    minutos=$(echo $linea | cut -d 'm' -f 1 | cut -d ' ' -f 2)
    segundos=$(echo $linea | cut -d 'm' -f 2 | cut -d 's' -f 1)
    segundos=$(echo $segundos | tr ',' '.')

    # convertir a segundos
    tiempo=$(echo "$minutos * 60 + $segundos" | bc -l)
    tiempo=$(printf "%.3f" "$tiempo")

    # escribir tiempo
    echo $tiempo
}

temp1=$(mktemp)
temp2=$(mktemp)
n1=$(cat $1)
n2=$(cat $2)
echo "Lanzando $n1 y $n2 en segundo plano..."

# Obtiene los nombres de los programas y del archivo de salida de los argumentos
programa1="sh $1"
programa2="sh $2"
archivo_salida=$3

# Ejecuta el programa 1 en segundo plano
$programa1 > $temp1 &
pid_programa1=$!
codigoerror1=$?


# Ejecuta el programa 2 en segundo plano
$programa2 > $temp2 &
pid_programa2=$!
codigoerror2=$?

echo "pid1 $pid_programa1 ; codigoerror1 $codigoerror1"
echo "pid2 $pid_programa2 ; codigoerror1 $codigoerror2"

# Espera a que uno de los programas termine
wait -n $pid_programa1 $pid_programa2

# Espera a que el otro programa termine
wait

#dar tiempo para que terminen de escribir los datos en el fichero de output
sleep 10


tiempo_programa1=$(obtener_tiempo $temp1)
tiempo_programa2=$(obtener_tiempo $temp2)

tiempo_programa1=$(printf "%0.3f" "$tiempo_programa1")
tiempo_programa2=$(printf "%0.3f" "$tiempo_programa2")


# Escribe los tiempos de finalización en el archivo de salida
echo "$tiempo_programa1":"$tiempo_programa2" > $archivo_salida
echo "$tiempo_programa1":"$tiempo_programa2"