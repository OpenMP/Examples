/*
* @@name:	private.3
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	pre_omp_3.0
*/
#include <assert.h>
void priv_example3()
{
  int i, a;

  #pragma omp parallel private(a)
  {
     a = 1;
    #pragma omp parallel for private(a)
      for (i=0; i<10; i++)
     {
       a = 2;
     }
    assert(a == 1);
  }
}
