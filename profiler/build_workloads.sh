PARSEC_DIR="/home/borja/Downloads/parsec-benchmark-master/bin/parsecmgmt"

echo $PARSEC_DIR -a build -p parsec.blackscholes -c gcc-serial
sleep 3
$PARSEC_DIR -a build -p parsec.blackscholes -c gcc-serial
sleep 3

echo $PARSEC_DIR -a build -p parsec.blackscholes -c gcc-pthreads
sleep 3
$PARSEC_DIR -a build -p parsec.blackscholes -c gcc-pthreads
sleep 3

###########


echo $PARSEC_DIR -a build -p parsec.bodytrack -c gcc-serial
sleep 3
$PARSEC_DIR -a build -p parsec.bodytrack -c gcc-serial
sleep 3

echo $PARSEC_DIR -a build -p parsec.bodytrack -c gcc-pthreads
sleep 3
$PARSEC_DIR -a build -p parsec.bodytrack -c gcc-pthreads
sleep 3

#########

echo $PARSEC_DIR -a build -p parsec.fluidanimate -c gcc-serial
sleep 3
$PARSEC_DIR -a build -p parsec.fluidanimate -c gcc-serial
sleep 3

echo $PARSEC_DIR -a build -p parsec.fluidanimate -c gcc-pthreads
sleep 3
$PARSEC_DIR -a build -p parsec.fluidanimate -c gcc-pthreads
sleep 3


#########

echo $PARSEC_DIR -a build -p parsec.freqmine -c gcc-serial
sleep 3
$PARSEC_DIR -a build -p parsec.freqmine -c gcc-serial
sleep 3

echo $PARSEC_DIR -a build -p parsec.freqmine -c gcc-openmp
sleep 3
$PARSEC_DIR -a build -p parsec.freqmine -c gcc-openmp
sleep 3

##########

echo $PARSEC_DIR -a build -p parsec.streamcluster -c gcc-serial
sleep 3
$PARSEC_DIR -a build -p parsec.streamcluster -c gcc-serial
sleep 3


echo $PARSEC_DIR -a build -p parsec.streamcluster -c gcc-pthreads
sleep 3
$PARSEC_DIR -a build -p parsec.streamcluster -c gcc-pthreads
sleep 3

#############
