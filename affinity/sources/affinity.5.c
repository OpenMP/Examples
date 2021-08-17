/*
* @@name:	affinity.5c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_5.1
*/
#if _OPENMP  < 202011
#define primary master
#endif

void work();
int main()
{
#pragma omp parallel proc_bind(primary) num_threads(4)
   {
      work();
   }
   return 0;
}
