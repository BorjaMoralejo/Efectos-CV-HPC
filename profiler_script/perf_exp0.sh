PARSEC_DIR="/home/borja/Downloads/parsec-benchmark-master/bin/parsecmgmt"

W1_n="blackscholes"
W1=$PARSEC_DIR" -a run -p parsec.blackscholes -c gcc-serial -i native"


eventos="instructions,cycles,cache-misses,cache-references,page-faults"
eventos="instructions,cycles,bus-cycles,page-faults,cache-misses,cache-references,LLC-load-misses,LLC-loads,LLC-stores,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,cs,migrations"
#eventos="instructions,cycles,stalled-cycles-frontend,bus-cycles,page-faults,cache-misses,cache-references,LLC-load-misses,LLC-loads,LLC-stores,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,cs,migrations"
shell="tempShell.sh"
echo $W1 > $shell
#sh ./perf_launcher.sh $W1_n Zero 0000001 20 $shell $eventos
sh ./perf_launcher.sh $W1_n Zero 0000001 16 $shell n
sh ./perf_launcher.sh $W1_n Zero 0000001 16 $shell $eventos

rm $shell
