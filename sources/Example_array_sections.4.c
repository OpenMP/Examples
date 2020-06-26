/*
* @@name:	array_sections.4c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
void foo ()
{
   int A[30], *p;
#pragma omp target data map( A[0:10] )
{
   p = &A[0];
   #pragma omp target map( p[3:7] )
   {
      A[2] = 0;
      p[8] = 0;
      A[8] = 1;
   }
}
}
