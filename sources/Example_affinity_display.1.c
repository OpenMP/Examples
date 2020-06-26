/*
* @@name: affinity_display.1.c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include <stdio.h>
#include <omp.h>

int main(void){                     //MAX threads = 8, single socket system

   omp_display_affinity(NULL);  //API call-- Displays Affinity of Master Thread

// API CALL OUTPUT (default format): 
//team_num= 0, nesting_level= 0, thread_num= 0, thread_affinity= 0,1,2,3,4,5,6,7


                   // OMP_DISPLAY_AFFINITY=TRUE, OMP_NUM_THREADS=8
   #pragma omp parallel num_threads(omp_get_num_procs())
   {
     if(omp_get_thread_num()==0) 
        printf("1st Parallel Region -- Affinity Reported \n");

         // DISPLAY OUTPUT (default format) has been sorted:
         // team_num= 0, nesting_level= 1, thread_num= 0, thread_affinity= 0
         // team_num= 0, nesting_level= 1, thread_num= 1, thread_affinity= 1
         // ...
         // team_num= 0, nesting_level= 1, thread_num= 7, thread_affinity= 7

     // doing work here
   }

   #pragma omp parallel num_threads( omp_get_num_procs() )
   {
     if(omp_get_thread_num()==0) 
        printf("%s%s\n","Same Affinity as in Previous Parallel Region",
                        " -- no Affinity Reported\n");

         // NO AFFINITY OUTPUT:  
         //(output in 1st parallel region only for OMP_DISPLAY_AFFINITY=TRUE)
     
     // doing more work here
   }

                       // Report Affinity for 1/2 number of threads
   #pragma omp parallel num_threads( omp_get_num_procs()/2 )
   {
     if(omp_get_thread_num()==0) 
        printf("Report Affinity for using 1/2 of max threads.\n");
    
         // DISPLAY OUTPUT (default format) has been sorted:
         // team_num= 0, nesting_level= 1, thread_num= 0, thread_affinity= 0,1
         // team_num= 0, nesting_level= 1, thread_num= 1, thread_affinity= 2,3
         // team_num= 0, nesting_level= 1, thread_num= 2, thread_affinity= 4,5
         // team_num= 0, nesting_level= 1, thread_num= 3, thread_affinity= 6,7

     // do work
   }

   return 0;
}
