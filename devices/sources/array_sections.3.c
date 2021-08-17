/*
* @@name:	array_sections.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
void foo ()
{
   int A[30], *p;
#pragma omp target data map( A[0:4] )
{
   p = &A[0];
   #pragma omp target map( p[7:20] )
   {
      A[2] = 0;
      p[8] = 0;
   }
}
}
