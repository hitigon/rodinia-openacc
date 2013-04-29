// #ifdef __cplusplus
// extern "C" {
// #endif

//========================================================================================================================================================================================================200
//	DEFINE/INCLUDE
//========================================================================================================================================================================================================200

//======================================================================================================================================================150
//	LIBRARIES
//======================================================================================================================================================150

#include <stdlib.h>									// (in directory known to compiler)			needed by malloc
#include <stdio.h>									// (in directory known to compiler)			needed by printf, stderr
#include <string.h>

//======================================================================================================================================================150
//	COMMON
//======================================================================================================================================================150

#include "../common.h"								// (in directory provided here)

//======================================================================================================================================================150
//	UTILITIES
//======================================================================================================================================================150

#include "../util/timer/timer.h"					// (in directory provided here)

//========================================================================================================================================================================================================200
//	KERNEL_CPU FUNCTION
//========================================================================================================================================================================================================200

void 
kernel_cpu(	int cores_arg,

			record *records,
            long int records_mem,
			knode *knodes,
			long int knodes_elem,

			int order,
			long int maxheight,
			int count,

			long int *currKnode,
			long int *offset,
			int *keys,
			record *ans)
{

	//======================================================================================================================================================150
	//	Variables
	//======================================================================================================================================================150

	// timer
	long long time0;
	long long time1;
	long long time2;

	time0 = get_time();

	//======================================================================================================================================================150
	//	MCPU SETUP
	//======================================================================================================================================================150

	int threadsPerBlock;
	threadsPerBlock = order < 1024 ? order : 1024;

	time1 = get_time();

	//======================================================================================================================================================150
	//	PROCESS INTERACTIONS
	//======================================================================================================================================================150

	// private thread IDs
	int thid;
	int bid;
	int i;

    
	// process number of querries
#pragma acc kernels copy(records[0:records_mem], knodes[0:knodes_elem], currKnode[0:count], offset[0:count], keys[0:count], ans[0:count]) 
{
#pragma acc loop independent
	for(bid = 0; bid < count; bid++){

		// process levels of the tree
        /*for(i = 0; i < maxheight; i++){*/

			// process all leaves at each level
#pragma acc loop independent
/*#pragma hmppcg nbiter min 1*/
			for(thid = 0; thid < threadsPerBlock; thid++){
                // if value is between the two keys
                for(i = 0; i < maxheight; i++){
                    if((knodes[currKnode[bid]].keys[thid]) <= keys[bid] && (knodes[currKnode[bid]].keys[thid+1] > keys[bid])){
                    // this conditional statement is inserted to avoid crush due to but in original code
                    // "offset[bid]" calculated below that addresses knodes[] in the next iteration goes outside of its bounds cause segmentation fault
                    // more specifically, values saved into knodes->indices in the main function are out of bounds of knodes that they address
                        if(knodes[offset[bid]].indices[thid] < knodes_elem){
                            offset[bid] = knodes[offset[bid]].indices[thid];
                        }
                    }

                    if (thid == 0) {
                        currKnode[bid] = offset[bid];
                    }

                }

                if(knodes[currKnode[bid]].keys[thid] == keys[bid]){
                    ans[bid].value = records[knodes[currKnode[bid]].indices[thid]].value;
                }
			}

			// set for next tree level
            /*currKnode[bid] = offset[bid];*/

        /*}*/

		//At this point, we have a candidate leaf node which may contain
		//the target record.  Check each key to hopefully find the record
		// process all leaves at each level
/*#pragma acc loop independent*/
        /*for(thid = 0; thid < threadsPerBlock; thid++){*/

            /*if(knodes[currKnode[bid]].keys[thid] == keys[bid]){*/
                /*ans[bid].value = records[knodes[currKnode[bid]].indices[thid]].value;*/
            /*}*/

        /*}*/

	}
}
	time2 = get_time();

	//======================================================================================================================================================150
	//	DISPLAY TIMING
	//======================================================================================================================================================150

	printf("Time spent in different stages of CPU/MCPU KERNEL:\n");

	printf("%15.12f s, %15.12f % : MCPU: SET DEVICE\n",					(float) (time1-time0) / 1000000, (float) (time1-time0) / (float) (time2-time0) * 100);
	printf("%15.12f s, %15.12f % : CPU/MCPU: KERNEL\n",					(float) (time2-time1) / 1000000, (float) (time2-time1) / (float) (time2-time0) * 100);

	printf("Total time:\n");
	printf("%.12f s\n", 												(float) (time2-time0) / 1000000);

}

//========================================================================================================================================================================================================200
//	END
//========================================================================================================================================================================================================200

// #ifdef __cplusplus
// }
// #endif

