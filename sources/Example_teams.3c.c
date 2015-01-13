/*
* @@name:	teams.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
float dotprod(float B[], float C[], int N)
{
   float sum = 0;
   int i;
   #pragma omp target teams map(to: B[0:N], C[0:N])
   #pragma omp distribute parallel for reduction(+:sum)
   for (i=0; i<N; i++)
      sum += B[i] * C[i];
   return sum;
}
