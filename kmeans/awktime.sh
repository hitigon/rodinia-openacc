#!/bin/sh
echo "GPU Time of Kernels:"
awk '$2!="memcpyDtoH" && $2!="memcpyHtoD" {sum+=$5} END {print sum*10e-6}' cuda_profile_0.log
echo "GPU Time of Memory:"
awk '$2=="memcpyDtoH" || $2=="memcpyHtoD" {sum+=$5} END {print sum*10e-6}' cuda_profile_0.log
echo "CPU Time of Kernels:"
awk '$2!="memcpyDtoH" && $2!="memcpyHtoD" {sum+=$8} END {print sum*10e-6}' cuda_profile_0.log
echo "CPU Time of Memory:"
awk '$2=="memcpyDtoH" || $2=="memcpyHtoD" {sum+=$8} END {print sum*10e-6}' cuda_profile_0.log
