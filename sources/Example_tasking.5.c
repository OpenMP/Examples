/*
* @@name:	tasking.5c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.0
*/
#define LARGE_NUMBER 10000000
double item[LARGE_NUMBER];
extern void process(double);

int main()
{
#pragma omp parallel
  {
    #pragma omp single
    {
      int i;
      for (i=0; i<LARGE_NUMBER; i++)
             #pragma omp task    // i is firstprivate, item is shared
                  process(item[i]);
    }
  }
}
