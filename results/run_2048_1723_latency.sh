#!/bin/bash

target_dir="../build/bin/"
target_file="aff3ct"
output_dir="./SCL/"

n="2048"
k="1723"
crc_type="32-GZIP"
nodes="{R0,R0L,R1,REP,REPL,SPC4+}"
sim_num="10000"


method0="FAST"
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method

list_size="8"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

list_size="16"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

list_size="32"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

list_size="64"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

method1="FASTCS"
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method

list_size="8"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

list_size="16"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

list_size="32"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file

list_size="64"
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method"_"$nodes".txt"

echo "L="$list_size
$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "4.25" -M "4.3" -s "0.25" -S 42 -t 1 -n $sim_num --sim-stats --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes -L $list_size >> $output_dir$output_file