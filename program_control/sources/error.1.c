/*
* @@name:       error.1c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.1
*/
#include <stdio.h>
#include   <omp.h>

int main(){

#pragma omp metadirective \
            when( implementation={vendor(gnu)}: nothing ) \
            default(error at(compilation) severity(fatal) \
                    message("GNU compiler required."))

  if( omp_get_num_procs() < 3 ){
    #pragma omp error at(runtime) severity(fatal) \
                      message("3 or more procs required.")
  }

  #pragma omp parallel master
  {
    // Give notice about master deprecation at compile time and run time.
    #pragma omp error at(compilation) severity(warning) \
                      message("Notice: master is deprecated.")
    #pragma omp error at(runtime) severity(warning) \
                      message("Notice: masked used next release.")

     printf(" Hello from thread number 0.\n");
  }

}
