#./rapl_power "../../Tercero/SCP/Proyecto/serie_tar/heat_s ../../Tercero/SCP/Proyecto/export2/card_other/card0" -n card0_e_e   >/dev/null
#./rapl_power "../../Tercero/SCP/Proyecto/serie_tar/heat_s ../../Tercero/SCP/Proyecto/export2/scale1/card_scale1" -n card_scale1_w_sleep2  >/dev/null
#./rapl_power "/usr/bin/sleep 1" -n sleepy_test  #>/dev/null

PARSEC_DIR="/home/borja/Downloads/parsec-benchmark-master/bin/parsecmgmt"

N=4

CPU_0_affinity=15 #00001111
CPU_1_affinity=15 #11110000
CPU_01_affinity= 15 #00110011 


# estas afinidades no estan balanceadas, deberia de ser N/2
CPU_JS1_affinity=2863311530  #32 x 10
CPU_JS2_affinity=1431655765  #101010101010101 32 x 01


# estas afinidades no estan balanceadas, deberia de ser N/2
# esto es la mascara para hyperthreading en una cpu
CPU_HT_0=15   #0101010101010101
CPU_HT_1=15   #1010101010101010


# estas afinidades no estan balanceadas, deberia de ser N/2
CPU_HT_JS1=15 #0000000001010101000000010101010
CPU_HT_JS2=15 #0000000000101010100000001010101


W1_n="blackscholes"
W1=$PARSEC_DIR" -a run -p parsec.blackscholes -c gcc-serial -i simlarge"
W1_4=$PARSEC_DIR" -a run -p parsec.blackscholes -c gcc-pthreads -i simlarge -n $N"

W2_n="bodytrack"
W2=$PARSEC_DIR" -a run -p parsec.bodytrack -c gcc-serial -i simlarge"
W2_4=$PARSEC_DIR" -a run -p parsec.bodytrack -c gcc-pthreads -i asimlarge -n $N"

W3_n="fluidanimate"
W3=$PARSEC_DIR" -a run -p parsec.fluidanimate -c gcc-serial -i simlarge"
W3_4=$PARSEC_DIR" -a run -p parsec.fluidanimate -c gcc-pthreads -i simlarge -n $N"

W4_n="freqmine"
W4=$PARSEC_DIR" -a run -p parsec.freqmine -c gcc-serial -i simlarge"
W4_4=$PARSEC_DIR" -a run -p parsec.freqmine -c gcc-openmp -i simlarge -n $N"

W5_n="streamcluster"
W5=$PARSEC_DIR" -a run -p parsec.streamcluster -c gcc-serial -i simlarge"
W5_4=$PARSEC_DIR" -a run -p parsec.streamcluster -c gcc-pthreads -i simlarge -n $N"



#W2="../../Tercero/SCP/Proyecto/serie_tar/heat_s ../../Tercero/SCP/Proyecto/export2/card_other/card0"
W1_3="/home/borja/Downloads/parsec-benchmark-master/bin/parsecmgmt -a run -p parsec.bodytrack -i simlarge -n 3"


#./rapl_power "/usr/bin/sleep 7" -n test -e "/usr/bin/sleep 2" #>/dev/null
#./rapl_power "../../Tercero/SCP/Proyecto/serie_tar/heat_s ../../Tercero/SCP/Proyecto/export2/card_other/card0" -n test2 -e "/home/borja/Downloads/parsec-benchmark-master/bin/parsecmgmt -a run -p parsec.bodytrack -i simlarge" >/dev/null
#./rapl_power "/usr/bin/sleep 10" -n nada_all2  #>/dev/null



## desactivar hyperthreading


##################
## Parte individual
## tener una base de donde poder sacar datos
./rapl_power "$W1" -n $W1_n"_ind" >/dev/null
./rapl_power "$W2" -n $W2_n"_ind" >/dev/null
./rapl_power "$W3" -n $W3_n"_ind" >/dev/null
./rapl_power "$W4" -n $W4_n"_ind" >/dev/null
./rapl_power "$W5" -n $W5_n"_ind" >/dev/null
#################

## Parte de parejas
# ver aumento de tiempo de ejecucion y otros contadores
./rapl_power "$W1" -n W1_2 -e "$W2" >/dev/null
./rapl_power "$W1" -n W1_3 -e "$W3" >/dev/null
./rapl_power "$W1" -n W1_4 -e "$W4" >/dev/null
./rapl_power "$W1" -n W1_5 -e "$W5" >/dev/null

./rapl_power "$W2" -n W2_3 -e "$W3" >/dev/null
./rapl_power "$W2" -n W2_4 -e "$W4" >/dev/null
./rapl_power "$W2" -n W2_5 -e "$W5" >/dev/null

./rapl_power "$W3" -n W1_2 -e "$W4" >/dev/null
./rapl_power "$W3" -n W1_2 -e "$W5" >/dev/null

./rapl_power "$W4" -n W1_2 -e "$W5" >/dev/null



####################################
## Parte paralelo
#####################################

## paralelo parejas en cpu de forma aislada
## en cpu 0
./rapl_power "$W1_4" -n $W1_n"_cpu0 -a $CPU_0_affinity" >/dev/null
./rapl_power "$W2_4" -n $W2_n"_cpu0 -a $CPU_0_affinity" >/dev/null
./rapl_power "$W3_4" -n $W3_n"_cpu0 -a $CPU_0_affinity" >/dev/null
./rapl_power "$W4_4" -n $W4_n"_cpu0 -a $CPU_0_affinity" >/dev/null
./rapl_power "$W5_4" -n $W5_n"_cpu0 -a $CPU_0_affinity" >/dev/null
## en cpu 1

#./rapl_power "$W1_4" -n $W1_n"_cpu1 -a $CPU_1_affinity" >/dev/null
#./rapl_power "$W2_4" -n $W2_n"_cpu1 -a $CPU_1_affinity" >/dev/null
#./rapl_power "$W3_4" -n $W3_n"_cpu1 -a $CPU_1_affinity" >/dev/null
#./rapl_power "$W4_4" -n $W4_n"_cpu1 -a $CPU_1_affinity" >/dev/null
#./rapl_power "$W5_4" -n $W5_n"_cpu1 -a $CPU_1_affinity" >/dev/null


## en paralelo en cpu separadas (job stripping)
./rapl_power "$W1_4" -n $W1_n"_ind_js -a $CPU_JS1_affinity" >/dev/null
./rapl_power "$W2_4" -n $W2_n"_ind_js -a $CPU_JS1_affinity" >/dev/null
./rapl_power "$W3_4" -n $W3_n"_ind_js -a $CPU_JS1_affinity" >/dev/null
./rapl_power "$W4_4" -n $W4_n"_ind_js -a $CPU_JS1_affinity" >/dev/null
./rapl_power "$W5_4" -n $W5_n"_ind_js -a $CPU_JS1_affinity" >/dev/null



###########################
## paralelo parejas
###########################


./rapl_power "$W1_4" -n W1_2_par -e "$W2_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null
./rapl_power "$W1_4" -n W1_3_par -e "$W3_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null 
./rapl_power "$W1_4" -n W1_4_par -e "$W4_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null
./rapl_power "$W1_4" -n W1_5_par -e "$W5_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null

./rapl_power "$W2_4" -n W2_3_par -e "$W3_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null
./rapl_power "$W2_4" -n W2_4_par -e "$W4_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null
./rapl_power "$W2_4" -n W2_5_par -e "$W5_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null

./rapl_power "$W3_4" -n W1_2_par -e "$W4_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null
./rapl_power "$W3_4" -n W1_2_par -e "$W5_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null

./rapl_power "$W4_4" -n W1_2_par -e "$W5_4" -a $CPU_0_affinity -b $CPU_1_affinity >/dev/null


####################################
## job stripping
####################################

./rapl_power "$W1_4" -n W1_2_stripping -e "$W2_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null
./rapl_power "$W1_4" -n W1_3_stripping -e "$W3_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null 
./rapl_power "$W1_4" -n W1_4_stripping -e "$W4_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null
./rapl_power "$W1_4" -n W1_5_stripping -e "$W5_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null

./rapl_power "$W2_4" -n W2_3_stripping -e "$W3_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null
./rapl_power "$W2_4" -n W2_4_stripping -e "$W4_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null
./rapl_power "$W2_4" -n W2_5_stripping -e "$W5_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null

./rapl_power "$W3_4" -n W1_2_stripping -e "$W4_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null
./rapl_power "$W3_4" -n W1_2_stripping -e "$W5_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null

./rapl_power "$W4_4" -n W1_2_stripping -e "$W5_4" -a $CPU_JS1_affinity -b $CPU_JS2_affinity >/dev/null

## activar hyperthreading

###############################
## hyperthreading en una sola cpu
################################
./rapl_power "$W1_4" -n W1_2_HT -e "$W2_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null
./rapl_power "$W1_4" -n W1_3_HT -e "$W3_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null 
./rapl_power "$W1_4" -n W1_4_HT -e "$W4_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null
./rapl_power "$W1_4" -n W1_5_HT -e "$W5_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null

./rapl_power "$W2_4" -n W2_3_HT -e "$W3_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null
./rapl_power "$W2_4" -n W2_4_HT -e "$W4_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null
./rapl_power "$W2_4" -n W2_5_HT -e "$W5_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null

./rapl_power "$W3_4" -n W1_2_HT -e "$W4_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null
./rapl_power "$W3_4" -n W1_2_HT -e "$W5_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null

./rapl_power "$W4_4" -n W1_2_HT -e "$W5_4" -a $CPU_HT_0 -b $CPU_HT_1 >/dev/null

############################
## job stripping e hyperthreading
############################
./rapl_power "$W1_4" -n W1_2_HT_JS -e "$W2_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null
./rapl_power "$W1_4" -n W1_3_HT_JS -e "$W3_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null 
./rapl_power "$W1_4" -n W1_4_HT_JS -e "$W4_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null
./rapl_power "$W1_4" -n W1_5_HT_JS -e "$W5_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null

./rapl_power "$W2_4" -n W2_3_HT_JS -e "$W3_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null
./rapl_power "$W2_4" -n W2_4_HT_JS -e "$W4_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null 
./rapl_power "$W2_4" -n W2_5_HT_JS -e "$W5_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null

./rapl_power "$W3_4" -n W1_2_HT_JS -e "$W4_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null
./rapl_power "$W3_4" -n W1_2_HT_JS -e "$W5_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null

./rapl_power "$W4_4" -n W1_2_HT_JS -e "$W5_4" -a $CPU_HT_JS1 -b $CPU_HT_JS2 >/dev/null

#############################

