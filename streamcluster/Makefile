TARGET_G = sc_gpu

all: gpu 

gpu:
	hmpp --openacc-target CUDA --codelet-required gcc -std=c99 -O3 -o $(TARGET_G) streamcluster.c -lm
	#gcc -std=c99 -O3 -o $(TARGET_G) streamcluster.c -lm

clean:
	rm -f *.o *~ *.txt sc_gpu *.hmf* *.log *.out
	
