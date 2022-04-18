/*
* @@name:       ompt_start.1
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.0
*/
#include <stdio.h>
#include <omp.h>
#include <omp-tools.h>

ompt_start_tool_result_t *ompt_start_tool(
unsigned int omp_version,
const char *runtime_version
){
  if(omp_version != _OPENMP)
    printf("Warning: OpenMP runtime version (%i) "
           "does not match the compile time version (%i)"
           " for runtime identifying as %s\n",
           omp_version, _OPENMP, runtime_version);
  // Returning NULL will disable this as an OMPT tool,
  // allowing other tools to be loaded
  return NULL;
}

int main(void){
  printf("Running with %i threads\n", omp_get_max_threads());
  return 0;
}
