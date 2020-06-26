/*
* @@name:       target_ptr_map.3a.c
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     failure
* @@version:	omp_5.0
*/
#define N 100

int x[N], y[N];
#pragma omp declare target
int *p1;
#pragma omp end declare target
int *p2;

int foo()
{
  p1 = &x[0];
  p2 = &y[0];

  // Explicitly map array section x[:N]
  #pragma omp target enter data map(x[:N])

  #pragma omp target  // as if .. map(p1) map(p1[:0]) map(p2[:0]) map(y)
  {
    // Accessing the mapped arrays x,y is OK here.
    x[0] = 1;
    y[1] = 2;

    // Pointer attachment for p1 does not occur here
    //   because p1[:0] does not allocate a new array section and
    //   array x is present on the target construct as it was mapped
    //   before by the target enter data directive.
    p1[0] = 3;      // accessing p1 is undefined

    // The initial value of p2 in the target region is undefined
    //   because map(y) may occur after map(p2[:0]).
    p2[1] = 4;      // accessing p2 is undefined
  }

  return 0;
}
