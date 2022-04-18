/*
* @@name:	display_env.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.1
*/
#include <omp.h>

//implementers: customize debug routines for app debugging
int debug(){ return 1; }
int debug_omp_verbose(){ return 0; }

int main()
{
   if( debug() ) omp_display_env( debug_omp_verbose() );
   // ...
   return 0;
}
