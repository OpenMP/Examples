/*
* @@name: metadirective.1c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/

#define N 100
#include <stdio.h>

int main()
{
   int v1[N], v2[N], v3[N];
   for(int i=0; i<N; i++){ v1[i]=(i+1); v2[i]=-(i+1); }

   #pragma omp target map(to:v1,v2) map(from:v3) device(0)
   #pragma omp metadirective \
                   when(   device={arch("nvptx")}: teams loop) \
                   default(                     parallel loop)
     for (int i= 0; i< N; i++)  v3[i] = v1[i] * v2[i];

   printf(" %d  %d\n",v3[0],v3[N-1]); //output: -1  -10000 

   return 0;
}
