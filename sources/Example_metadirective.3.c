/*
* @@name: metadirective.3c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include <stdio.h>
#include  <math.h>
#define      N 1000

#pragma omp declare target
void exp_pi_diff(double *d, double my_pi){
   #pragma omp metadirective \
               when( construct={target}: distribute parallel for  ) \
               default(                             parallel for simd)
   for(int i = 0; i<N; i++) d[i] = exp( (M_PI-my_pi)*i );
}
#pragma omp end declare target

int main()
{
  //Calculates sequence of exponentials: (M_PI-my_pi) * index
  //M_PI is from math.h, and my_pi is user provided.

  double d[N];
  double my_pi=3.14159265358979e0;

      #pragma omp target teams map(tofrom: d[0:N])
      exp_pi_diff(d,my_pi);
                                           // value should be near 1
      printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311

      exp_pi_diff(d,my_pi);                // value should be near 1
      printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311
}
