/*
* @@name:	affinity.4
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_4.0
*/
void work();
void foo()
{
  #pragma omp parallel num_threads(16) proc_bind(close)
  {
    work();
  }
}
