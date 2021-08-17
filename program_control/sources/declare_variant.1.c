/*
* @@name: declare_variant.1c
* @@type: C
* @@compilable: yes
* @@linkable: yes 
* @@expect: success
* @@version: omp_5.0
*/

#define N 100
#include <stdio.h>
#include <omp.h>

void p_vxv(int *v1,int *v2,int *v3,int n);
void t_vxv(int *v1,int *v2,int *v3,int n);

#pragma omp declare variant( p_vxv ) match( construct={parallel} )
#pragma omp declare variant( t_vxv ) match( construct={target}   )
void vxv(int *v1,int *v2,int *v3,int n)     // base function
{
   for (int i= 0; i< n; i++)  v3[i] = v1[i] * v2[i];
}

void p_vxv(int *v1,int *v2,int *v3,int n)   // function variant
{
   #pragma omp for
   for (int i= 0; i< n; i++)  v3[i] = v1[i] * v2[i]*3;
}

#pragma omp declare target
void t_vxv(int *v1,int *v2,int *v3,int n)   // function variant
{
   #pragma omp distribute simd
   for (int i= 0; i< n; i++)  v3[i] = v1[i] * v2[i]*2;
}
#pragma omp end declare target

int main()
{
   int v1[N], v2[N], v3[N];
   for(int i=0; i<N; i++){ v1[i]=(i+1); v2[i]=-(i+1); v3[i]=0; }   //init

   #pragma omp parallel
   {
      vxv(v1,v2,v3,N);
   }
   printf(" %d  %d\n",v3[0],v3[N-1]); //from p_vxv --  output: -3  -30000

   #pragma omp target teams map(to: v1[:N],v2[:N]) map(from: v3[:N])
   {
      vxv(v1,v2,v3,N);
   }
   printf(" %d  %d\n",v3[0],v3[N-1]); //from t_vxv --  output: -2  -20000

   vxv(v1,v2,v3,N);
   printf(" %d  %d\n",v3[0],v3[N-1]); //from   vxv --  output: -1  -10000

   return 0;
}
