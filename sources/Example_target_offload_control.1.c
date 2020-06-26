/*
* @@name: target_offload_control.1c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/

#include    <omp.h>
#include  <stdio.h>
#include  <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum offload_policy
{MANDATORY, DISABLED, DEFAULT, UNKNOWN, NOTSET} offload_policy_t;


offload_policy_t get_offload_policy()
{  
   char *env, *end; 
   size_t n;

   env = getenv("OMP_TARGET_OFFLOAD");
   if(env  == NULL) return NOTSET;
  
   end = env + strlen(env);                //Find trimmed beginning/end
   while (      *env && isspace(*(env  )) ) env++;
   while (end != env && isspace(*(end-1)) ) end--;
   n = (int)(end - env);

                    //Find ONLY string -nothing more, case insensitive
   if      (n == 9 && !strncasecmp(env, "MANDATORY",n)) return MANDATORY;
   else if (n == 8 && !strncasecmp(env, "DISABLED" ,n)) return DISABLED ;
   else if (n == 7 && !strncasecmp(env, "DEFAULT"  ,n)) return DEFAULT  ;
   else                                                 return UNKNOWN  ;
}

 
int main()
{
   int i;
   int device_num, on_init_dev;

                              // get policy from OMP_TARGET_OFFLOAD variable
   offload_policy_t policy = get_offload_policy();

   if(_OPENMP< 201811)
   {
      printf("Warning: OMP_TARGET_OFFLOAD NOT supported by VER. %d\n",_OPENMP );
      printf("         If OMP_TARGET_OFFLOAD is set, it will be ignored.\n");
   }

                              // Set target device number to an unavailable
                              // device to test offload policy.
   device_num = omp_get_num_devices() + 1;

                                   // Policy:
   printf("OMP_TARGET_OFFLOAD Policy:  ");
   if     (policy==MANDATORY) printf("MANDATORY-Terminate if dev. not avail\n");
   else if(policy==DISABLED ) printf("DISABLED -(if supported) Only on Host\n");
   else if(policy==DEFAULT  ) printf("DEFAULT  -On host if device not avail\n");
   else if(policy==UNKNOWN  ) printf("OMP_TARGET_OFFLOAD has unknown value\n" );
   else if(policy==NOTSET   ) printf("OMP_TARGET_OFFLOAD not set\n" );

     
   on_init_dev = 1;
                                         // device# out of range--not supported
   #pragma omp target device(device_num) map(tofrom: on_init_dev)
     on_init_dev=omp_is_initial_device();

   if (policy == MANDATORY && _OPENMP >= 201811)
      printf("ERROR: OpenMP 5.0 implementation ignored MANDATORY policy.\n");

   printf("Target region executed on init dev %s\n", on_init_dev ? "TRUE":"FALSE");

   return 0;
}
