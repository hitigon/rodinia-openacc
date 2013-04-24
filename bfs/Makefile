# C compiler
CC = hmpp --openacc-target=CUDA --codelet-required gcc
CC_FLAGS = -std=c99 -g -O2

bfs: 
	$(CC) $(CC_FLAGS) -o bfs bfs.c

clean:
	rm -f bfs result.txt *.out *.log *.hmf* 
