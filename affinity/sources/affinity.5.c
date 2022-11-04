/*
* @@name:	affinity.5
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
void work();
int main()
{
#pragma omp parallel proc_bind(primary) num_threads(4)
   {
      work();
   }
   return 0;
}
