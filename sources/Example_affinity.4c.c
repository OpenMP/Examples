/*
* @@name:	affinity.4c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
void work();
void foo()
{
  #pragma omp parallel num_threads(16) proc_bind(close)
  {
    work();
  }
}
