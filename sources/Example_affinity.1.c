/*
* @@name:	affinity.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/

void work();

int main() 
{

#pragma omp parallel proc_bind(spread) num_threads(4)
   {
      work();
   }

   return 0;

}
