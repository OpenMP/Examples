/*
* @@name: affinity_display.3.c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include <stdio.h>
#include <stdlib.h>  // also null is in <stddef.h>
#include <stddef.h>
#include <string.h>
#include <omp.h>

#define FORMAT_STORE   80
#define BUFFER_STORE   80

int main(void){

   int i, n, thrd_num, max_req_store;
   size_t nchars;

   char default_format[FORMAT_STORE];
   char my_format[]  = "host=%20H thrd_num=%0.4n binds_to=%A";
   char **buffer;


// CODE SEGMENT 1         AFFINITY FORMAT

//                        Get and Display Default Affinity Format

   nchars = omp_get_affinity_format(default_format,(size_t)FORMAT_STORE);
   printf("Default Affinity Format is: %s\n",default_format);

   if(nchars >= FORMAT_STORE){
      printf("Caution: Reported Format is truncated.  Increase\n");
      printf("         FORMAT_STORE to %d.\n", nchars+1);
   }

//                        Set Affinity Format

   omp_set_affinity_format(my_format);
   printf("Affinity Format set to: %s\n",my_format);


// CODE SEGMENT 2         CAPTURE AFFINITY 

//                        Set up buffer for affinity of n threads

   n = omp_get_num_procs();
   buffer = (char **)malloc( sizeof(char *) * n );
   for(i=0;i<n;i++){ buffer[i]=(char *)malloc( sizeof(char) * BUFFER_STORE); }

//                        Capture Affinity using Affinity Format set above.
//                        Use max reduction to check size of buffer areas
   max_req_store = 0;
   #pragma omp parallel private(thrd_num,nchars) reduction(max:max_req_store)
   {
      if(omp_get_num_threads()>n) exit(1); //safety: don't exceed # of buffers

      thrd_num=omp_get_thread_num();
      nchars=omp_capture_affinity(buffer[thrd_num],(size_t)BUFFER_STORE,NULL);
      if(nchars > max_req_store) max_req_store=nchars;

      // ...
   }

   for(i=0;i<n;i++){ 
      printf("thrd_num= %d, affinity: %s\n", i,buffer[i]); 
   }
         // For 4 threads with OMP_PLACES='{0,1},{2,3},{4,5},{6,7}'
         // Format    host=%20H thrd_num=%0.4n binds_to=%A

         // affinity: host=hpc.cn567            thrd_num=0000 binds_to=0,1
         // affinity: host=hpc.cn567            thrd_num=0001 binds_to=2,3
         // affinity: host=hpc.cn567            thrd_num=0002 binds_to=4,5
         // affinity: host=hpc.cn567            thrd_num=0003 binds_to=6,7


   if(max_req_store>=BUFFER_STORE){
      printf("Caution: Affinity string truncated.  Increase\n");
      printf("         BUFFER_STORE to %d\n",max_req_store+1);
   }

   for(i=0;i<n;i++) free(buffer[i]);
   free (buffer);

   return 0;
}
