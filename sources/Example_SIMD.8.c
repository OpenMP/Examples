/*
* @@name:	SIMD.8c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
#include <math.h>
 
int   P[1000];
float A[1000];
 
float do_work(float *arr)
{
  float pri;
  int i;
#pragma omp simd lastprivate(pri)
  for (i = 0; i < 999; ++i) {
    int j = P[i];
 
    pri = 0.5f;
    if (j % 2 == 0) {
      pri = A[j+1] + arr[i];
    }
    A[j] = pri * 1.5f;
    pri = pri + A[j];
  }
  return pri;
}
 
int main(void)
{
  float pri, arr[1000];
  int i;
 
  for (i = 0; i < 1000; ++i) {
     P[i]   = i;
     A[i]   = i * 1.5f;
     arr[i] = i * 1.8f;
  }
  pri = do_work(&arr[0]);
  if (pri == 8237.25) {
    printf("passed: result pri = %7.2f (8237.25) \n", pri);
  }
  else {
    printf("failed: result pri = %7.2f (8237.25) \n", pri);
  }
  return 0;
}
