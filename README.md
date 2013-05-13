Testing Environment:
--------------------

OS: Linux

Compiler: hmpp, gcc

Resource Manager: SLURM

Compile:
--------

type 'make'

if you are using PGI, you may need to adjust the codes and Makefiles

Usage:
------

type './run'

if you are not using SLURM, please remove 'srun -N1 --gres=gpu:1' in the script file

Profile:
-------

Please set the environment variable:
export CUDA_PROFILE=1

To-do:
-----

Nothing for now
