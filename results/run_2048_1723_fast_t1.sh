#!/bin/bash

target_dir="../build/bin/"
target_file="aff3ct"
output_dir="./SCL/"

n="2048"
k="1723"
crc_type="32-GZIP"
num_thread="1"


echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method

target_dir="../build/bin/"
target_file="aff3ct"
output_dir="./SCL/"

method0="FAST"
method1="FASTCS"
method2="FASTCS1"
method3="FASTCS2"
seed="42"
nodes4p="{R0,R0L,R1,REP,REPL,SPC_4+}"

###############################################################################################################
list_size="2"
echo "L="$list_size

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method3 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

###############################################################################################################
list_size="4"
echo "L="$list_size

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method3 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

###############################################################################################################
list_size="8"
echo "L="$list_size


echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method3 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

###############################################################################################################
list_size="16"
echo "L="$list_size


echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method3 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

###############################################################################################################
list_size="32"
echo "L="$list_size


echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "L="$list_size
echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "3.75" -M "4.3" -s "0.25" -e "100" --sim-seed $seed -t $num_thread --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method3 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

