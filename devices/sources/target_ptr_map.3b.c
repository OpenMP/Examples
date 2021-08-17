/*
* @@name:       target_ptr_map.3b.c
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     success
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

  // Explicitly map array section y[:N]
  #pragma omp target enter data map(y[:N])

  #pragma omp target map(x[:N]) map(p1[:N]) map(p2[:0])
  {
    // Accessing the mapped arrays x,y is OK here.
    x[0] = 1;
    y[1] = 2;

    // Pointer attachment for p1 occurs here when array x is mapped
    //   on the target construct (as p1 = &x[0] on the device)
    p1[0] = 3;      // accessing p1 is OK

    // p2 in the target region is initialized to &y[0]
    p2[1] = 4;      // accessing p2 is OK
  }

  return 0;
}
