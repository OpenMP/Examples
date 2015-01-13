/*
* @@name:	affinity.5c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
void work();
void main()
{
#pragma omp parallel proc_bind(master) num_threads(4)
   {
      work();
   }
}
