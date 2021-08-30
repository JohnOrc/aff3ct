#!/bin/bash

target_dir="../build/bin/"
target_file="aff3ct"
output_dir="./SCL/"

n="2048"
k="1723"
crc_type="32-GZIP"
method0="FASTCS"
method1="FASTCS1"
method2="FASTCS2"
method3="FAST"
nodes4="{R0,R0L,R1,REP,REPL,SPC_4}"
nodes4p="{R0,R0L,R1,REP,REPL,SPC_4+}"
nodesr1="{R0,R0L,R1,REP,REPL}"


###############################################################################################################
list_size="64"
echo "L="$list_size


echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodesr1".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodesr1 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodesr1".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodesr1 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodesr1".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodesr1 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method0
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method0"_"$nodes4p".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method0 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method1
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method1"_"$nodes4p".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method1 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method2
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method2"_"$nodes4p".txt"

# $target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodesr1".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodesr1 -L $list_size >> $output_dir$output_file

echo "processing: n = "$n" k = "$k" crc_type = "$crc_type" method = "$method3
output_file=$USER"_"$n"_"$k"_"$list_size"_"$crc_type"_"$method3"_"$nodes4p".txt"

$target_dir$target_file -p "32" --sim-type "BFER" -C "POLAR" -m "0.0" -M "4.3" -s "0.25" -e "100" --sim-meta $output_file --enc-fb-gen-method "GA" -K $k -N $n --src-type "RAND"  --chn-type "AWGN"  --dec-type "SCL" --dec-implem $method2 --dec-simd "INTRA" --crc-type $crc_type --crc-implem "FAST" --dec-polar-nodes $nodes4p -L $list_size >> $output_dir$output_file
