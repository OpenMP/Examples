/*
* @@name:	async_target.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
#pragma omp begin declare target
float F(float);
#pragma omp end declare target

#define N 1000000000
#define CHUNKSZ 1000000
void init(float *, int);
float Z[N];
void pipedF(){
   int C, i;
   init(Z, N);
   for (C=0; C<N; C+=CHUNKSZ){
      #pragma omp task shared(Z)
      #pragma omp target map(Z[C:CHUNKSZ])
      #pragma omp parallel for
      for (i=0; i<CHUNKSZ; i++) Z[i] = F(Z[i]);
   }
   #pragma omp taskwait
}
