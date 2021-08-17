/*
* @@name:	affinity.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
void work();
int main()
{
#pragma omp parallel proc_bind(close) num_threads(4)
   {
      work();
   }
   return 0;
}
