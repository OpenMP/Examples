/*
* @@name:	affinity.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
void work();
void main()
{
#pragma omp parallel proc_bind(close) num_threads(4)
   {
      work();
   }
}
