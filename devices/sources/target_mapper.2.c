/*
* @@name: target_mapper_map.2.c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_5.0
*/
#include <stdio.h>
//                   N MUST BE EVEN
#define N  100

  typedef struct dzmat
  {
     double r_m[N][N];
     double i_m[N][N];
  } dzmat_t;

  #pragma omp declare mapper( top_id: dzmat_t v) \
                      map(v.r_m[0:N/2][0:N],     \
                          v.i_m[0:N/2][0:N]      )

  #pragma omp declare mapper(bottom_id: dzmat_t v) \
                      map(v.r_m[N/2:N/2][0:N],     \
                          v.i_m[N/2:N/2][0:N]      )

void dzmat_init(dzmat_t *z, int is, int ie, int n);         //initialization
void host_add(  dzmat_t *a, dzmat_t *b, dzmat_t *c, int n); //matrix add: c=a+b


int main()
{
  dzmat_t a,b,c;
  int     is,ie;

  is=0; ie=N/2-1;       //top N/2 rows on device 0
  #pragma omp target map(mapper(top_id), tofrom: a,b) device(0) \
                     firstprivate(is,ie) nowait
  {
    dzmat_init(&a,is,ie,N);
    dzmat_init(&b,is,ie,N);
  }

  is=N/2; ie=N-1;       //bottom N/2 rows on device 1
  #pragma omp target map(mapper(bottom_id), tofrom: a,b) device(1) \
                     firstprivate(is,ie) nowait
  {
    dzmat_init(&a,is,ie,N);
    dzmat_init(&b,is,ie,N);
  }
 
  #pragma omp taskwait

  host_add(&a,&b,&c,N);
}
