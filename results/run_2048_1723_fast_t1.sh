#!/bin/bash

target_dir="../build/bin/"
target_file="aff3ct"
output_dir="./SCL/"

n="2048"
k="1723"
crc_type="32-GZIP"
method="fast"
nodes="{R0,R0L,R1,REP,REPL,SPC_4}"
num_thread="1"
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method

list_size="4"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$num_thread"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.1" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size -t $num_thread >> $output_dir$output_file


list_size="8"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$num_thread"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.1" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size -t $num_thread >> $output_dir$output_file


list_size="16"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$num_thread"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.1" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size -t $num_thread >> $output_dir$output_file


list_size="32"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$num_thread"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.1" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size -t $num_thread >> $output_dir$output_file
