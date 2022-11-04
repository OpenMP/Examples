/*
* @@name:	target_associate_ptr.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>
#include <omp.h>

#define CS 50
#define N  (CS*2)

int main() {
  int arr[N];
  int *dev_ptr;
  int dev;

  for (int i = 0; i < N; i++)
    arr[i] = i;

  dev = omp_get_default_device();

  // Allocate device memory
  dev_ptr = (int *)omp_target_alloc(sizeof(int) * CS, dev);

  // Loop over chunks
  for (int ioff = 0; ioff < N; ioff += CS) {

    // Associate device memory with one chunk of host memory
    omp_target_associate_ptr(&arr[ioff], dev_ptr,
                             sizeof(int) * CS, 0, dev);

    printf("before: arr[%d]=%d\n", ioff, arr[ioff]);

    // Update the device data
    #pragma omp target update to(arr[ioff:CS]) device(dev)

    // Explicit mapping of arr to make sure that we use the allocated 
    // and associated memory.  No host-device data update here.
    #pragma omp target map(tofrom : arr[ioff:CS]) device(dev)
      for (int i = 0; i < CS; i++) {
        arr[i+ioff]++;
      }

    // Update the host data
    #pragma omp target update from(arr[ioff:CS]) device(dev)

    printf("after: arr[%d]=%d\n", ioff, arr[ioff]);

    // Disassociate device pointer from the current chunk of host memory
    // before next use
    omp_target_disassociate_ptr(&arr[ioff], dev);
  }

  // Free device memory
  omp_target_free(dev_ptr, dev);

  return 0;
}
/* Outputs:
  before: arr[0]=0
  after: arr[0]=1
  before: arr[50]=50
  after: arr[50]=51
 */
