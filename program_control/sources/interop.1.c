/*
* @@name:       interop.1
* @@type:       C
* @@compilable: no
* @@linkable:   no
* @@expect:     success
* @@version:    omp_5.1
*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cublas_v2.h>
#include <cuda_runtime_api.h>

#define N 16384

void myVectorSet(int n, double s, double *x)
{
   for(int i=0; i<n; ++i) x[i] = s*(i+1);
}
void myDaxpy(int n, double s, double *x, double *y)
{
   for(int i=0; i<n; ++i) y[i] = s*x[i]+y[i];
}
void myDscal(int n, double s, double *x)
{
   for(int i=0; i<n; ++i) x[i] = s*x[i];
}


int main(){
  const  double scalar=2.0;
  double *x, *y, *d_x, *d_y;
  int    dev;

  omp_interop_t obj=omp_interop_none;
  intptr_t  type;

  // Async Memcpy requires pinned memory
  cudaMallocHost( (void**)&x,   N*sizeof(double) );
  cudaMallocHost( (void**)&y,   N*sizeof(double) );
  cudaMalloc(     (void**)&d_x, N*sizeof(double) );
  cudaMalloc(     (void**)&d_y, N*sizeof(double) );

  dev = omp_get_default_device();
  omp_target_associate_ptr(&x[0], d_x, sizeof(double)*N, 0, dev);
  omp_target_associate_ptr(&y[0], d_y, sizeof(double)*N, 0, dev);

  #pragma omp target nowait depend(out: x[0:N]) \
                     map(from: x[0:N]) device(dev)
  myVectorSet(N,  1.0, x);

  #pragma omp task depend(out: y[0:N])
  myVectorSet(N, -1.0, y);

  // get obj for syncing
  #pragma omp interop init(targetsync: obj) device(dev) \
                 depend(in: x[0:N]) depend(inout: y[0:N])

                                        //foreign rt id and string name
  int        id = (int  )omp_get_interop_int(obj, omp_ipr_fr_id,   NULL);
  char* rt_name = (char*)omp_get_interop_str(obj, omp_ipr_fr_name, NULL);

  if(obj != omp_interop_none && id == omp_ifr_cuda) {

    printf(" OpenMP working with %s runtime to execute cublas daxpy.\n",
           rt_name);
    cublasHandle_t handle;
    int rc;
    cublasCreate(&handle);

    cudaStream_t s=
        (cudaStream_t)omp_get_interop_ptr(obj, omp_ipr_targetsync, &rc);
    if(rc != omp_irc_success) {
       fprintf(stderr,"ERROR: Failed to get %s stream, rt error= %d.\n",
               rt_name, rc);
       if(rc == omp_irc_no_value)
          fprintf(stderr,
                  "Parameters valid, no meaningful value available.");
       exit(1);
    }

    cublasSetStream( handle,s );
    cudaMemcpyAsync( d_x, x, N*sizeof(double),
                     cudaMemcpyHostToDevice, s );
    cudaMemcpyAsync( d_y, y, N*sizeof(double),
                     cudaMemcpyHostToDevice, s );
    cublasDaxpy(     handle, N, &scalar, &d_x[0], 1, &d_y[0], 1 ) ;
    cudaMemcpyAsync( y, d_y, N*sizeof(double),
                     cudaMemcpyDeviceToHost, s );

  } else {     // Execute as OpenMP offload.

    printf(" Notice: Offloading myDaxpy to perform daxpy calculation.\n");

    #pragma omp target depend(inout: y[0:N]) depend(in: x[0:N]) nowait \
                       map(to: x[0:N]) map(tofrom: y[0:N]) device(dev)
    myDaxpy(N, scalar, x, y);

  }

   // This also ensures foreign tasks complete.
  #pragma omp interop destroy(obj) nowait depend(out: y[0:N])

  #pragma omp target depend(inout: x[0:N])
  myDscal(N, scalar, x);

  #pragma omp taskwait
  printf("(-1:-16384) %f:%f\n", y[0], y[N-1]);
  printf("(-2:-32768) %f:%f\n", x[0], x[N-1]);

}
