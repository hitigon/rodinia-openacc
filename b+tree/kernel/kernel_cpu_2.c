// #ifdef __cplusplus
// extern "C" {
// #endif

//========================================================================================================================================================================================================200
//	DEFINE/INCLUDE
//========================================================================================================================================================================================================200

//======================================================================================================================================================150
//	LIBRARIES
//======================================================================================================================================================150

#include <stdlib.h>									// (in directory known to compiler)
#include <stdio.h>

//======================================================================================================================================================150
//	COMMON
//======================================================================================================================================================150

#include "../common.h"								// (in directory provided here)

//======================================================================================================================================================150
//	UTILITIES
//======================================================================================================================================================150

#include "../util/timer/timer.h"					// (in directory provided here)	needed by timer

//======================================================================================================================================================150
//	HEADER
//======================================================================================================================================================150

#include "./kernel_cpu_2.h"							// (in directory provided here)

//========================================================================================================================================================================================================200
//	PLASMAKERNEL_GPU
//========================================================================================================================================================================================================200

void 
kernel_cpu_2(	int cores_arg,

				knode *knodes,
				long int knodes_elem,

				int order,
				long int maxheight,
				int count,

				long int *currKnode,
				long int *offset,
				long int *lastKnode,
				long int *offset_2,
				int *start,
				int *end,
				int *recstart,
				int *reclength)
{

	//======================================================================================================================================================150
	//	Variables
	//======================================================================================================================================================150

	// timer
	long long time0;
	long long time1;
	long long time2;

	// common variables
	int i;

	time0 = get_time();

	//======================================================================================================================================================150
	//	MCPU SETUP
	//======================================================================================================================================================150

	int threadsPerBlock;
	threadsPerBlock = order < 1024 ? order : 1024;

	

	//======================================================================================================================================================150
	//	PROCESS INTERACTIONS
	//======================================================================================================================================================150

	// private thread IDs
	int thid;
	int bid;

#pragma acc data copy(knodes[0:knodes_elem], currKnode[0:count], offset[0:count], lastKnode[0:count], offset_2[0:count], start[0:count], end[0:count], recstart[0:count], reclength[0:count])
{
	time1 = get_time();
#pragma acc kernels private(thid, bid, i)
{
#pragma acc loop independent
	// process number of querries
	for(bid = 0; bid < count; bid++){
#pragma acc loop independent
		for(thid = 0; thid < threadsPerBlock; thid++){

		    for(i = 0; i < maxheight; i++){
				if((knodes[currKnode[bid]].keys[thid] <= start[bid]) && (knodes[currKnode[bid]].keys[thid+1] > start[bid])){
					// this conditional statement is inserted to avoid crush due to but in original code
					// "offset[bid]" calculated below that later addresses part of knodes goes outside of its bounds cause segmentation fault
					// more specifically, values saved into knodes->indices in the main function are out of bounds of knodes that they address
					if(knodes[currKnode[bid]].indices[thid] < knodes_elem){
						offset[bid] = knodes[currKnode[bid]].indices[thid];
					}
				}
				if((knodes[lastKnode[bid]].keys[thid] <= end[bid]) && (knodes[lastKnode[bid]].keys[thid+1] > end[bid])){
					// this conditional statement is inserted to avoid crush due to but in original code
					// "offset_2[bid]" calculated below that later addresses part of knodes goes outside of its bounds cause segmentation fault
					// more specifically, values saved into knodes->indices in the main function are out of bounds of knodes that they address
					if(knodes[lastKnode[bid]].indices[thid] < knodes_elem){
						offset_2[bid] = knodes[lastKnode[bid]].indices[thid];
					}
				}

			    // set for next tree level
                if (thid == 0) {
			        currKnode[bid] = offset[bid];
			        lastKnode[bid] = offset_2[bid];
                }
			}


			if(knodes[currKnode[bid]].keys[thid] == start[bid]){
				recstart[bid] = knodes[currKnode[bid]].indices[thid];
			}

			if(knodes[lastKnode[bid]].keys[thid] == end[bid]){
				reclength[bid] = knodes[lastKnode[bid]].indices[thid] - recstart[bid]+1;
			}
        }
	}
}
	time2 = get_time();
}
	
	//======================================================================================================================================================150
	//	DISPLAY TIMING
	//======================================================================================================================================================150

	printf("Time spent in different stages of CPU/MCPU KERNEL:\n");

	printf("%15.12f s, %15.12f % : MCPU: SET DEVICE\n",					(float) (time1-time0) / 1000000, (float) (time1-time0) / (float) (time2-time0) * 100);
	printf("%15.12f s, %15.12f % : CPU/MCPU: KERNEL\n",					(float) (time2-time1) / 1000000, (float) (time2-time1) / (float) (time2-time0) * 100);

	printf("Total time:\n");
	printf("%.12f s\n", 												(float) (time2-time0) / 1000000);

} // main

//========================================================================================================================================================================================================200
//	END
//========================================================================================================================================================================================================200

// #ifdef __cplusplus
// }
// #endif
