/*
* @@name:	teams.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.5
*/
float dotprod(float B[], float C[], int N)
{
   float sum = 0;
   int i;
   #pragma omp target teams map(to: B[0:N], C[0:N]) \
                            defaultmap(tofrom:scalar) reduction(+:sum)
   #pragma omp distribute parallel for reduction(+:sum)
   for (i=0; i<N; i++)
      sum += B[i] * C[i];
   return sum;
}

/* Note:  The variable sum is now mapped with tofrom from the defaultmap
   clause on the combined target teams construct, for correct 
   execution with 4.5 (and pre-4.5) compliant compilers. See Devices Intro. 
 */
