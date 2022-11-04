/*
* @@name:	target_struct_map.2
* @@type:	C++
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.1
*/
#include <cstdio>
#include <cstdlib>
#define N 100

class SAXPY {
  private:
   float a, b, *p;
  public:
   float buffer[N];

   SAXPY(float arg_a, float arg_b){ a=arg_a; b=arg_b; }
   void driver();
   void saxpyfun(float *p);
};

#pragma omp begin declare target
void SAXPY::saxpyfun(float *q)
{
  for(int i=0; i<N; i++)
    buffer[i] = q[i]*a + b;
}
#pragma omp end declare target

void SAXPY::driver()
{
  p = (float *) malloc(N*sizeof(float));
  for(int i=0; i<N; i++) p[i]=i;

  #pragma omp target map(alloc:p) map(to:p[:N]) map(to:a,b) \
              map(from:buffer[:N])   // attach(p) to device_malloc()
  {
    saxpyfun(p);
  }

  free(p);
}

int main()
{
  SAXPY my_saxpy(2.0,4.0);

  my_saxpy.driver();

  printf(" %4.0f %4.0f\n", my_saxpy.buffer[0], my_saxpy.buffer[N-1]);
         //   4   202     <- output

  return 0;
}
