/*
* @@name: array_shaping.1.c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_5.0
*/
#pragma omp declare target
int do_work(double *a, int nx, int ny);
int other_work(double *a, int nx, int ny);
#pragma omp end declare target

void exch_data(double *a, int nx, int ny);

void array_shaping(double *a, int nx, int ny)
{
   // map data to device and do work
   #pragma omp target data map(a[0:nx*(ny+2)])
   {
      // do work on the device
      #pragma omp target  // map(a[0:nx*(ny+2)]) is optional here
      do_work(a, nx, ny);

      // update boundary points (two columns of 2D array) on the host
      // pointer is shaped to 2D array using the shape-operator
      #pragma omp target update from( (([nx][ny+2])a)[0:nx][1], \
                                      (([nx][ny+2])a)[0:nx][ny] )

      // exchange ghost points with neighbors
      exch_data(a, nx, ny);

      // update ghost points (two columns of 2D array) on the device
      // pointer is shaped to 2D array using the shape-operator
      #pragma omp target update to( (([nx][ny+2])a)[0:nx][0], \
                                    (([nx][ny+2])a)[0:nx][ny+1] )

      // perform other work on the device
      #pragma omp target  // map(a[0:nx*(ny+2)]) is optional here
      other_work(a, nx, ny);
   }
}
