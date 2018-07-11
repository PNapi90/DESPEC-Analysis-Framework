#!/bin/bash

cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0001.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0002.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0003.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0004.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0005.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0006.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0007.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0008.gm
cp 60Co_16_30_Det_400mV_4_1.gm 60Co_16_30_Det_400mV_4_0009.gm

for run in {1..9}
do
   runb=$(( $run + 1 ))
   for i in {0..9}
   do
      cp 60Co_16_30_Det_400mV_4_$runb.gm 60Co_16_30_Det_400mV_4_00${run}$i.gm
   done
done
