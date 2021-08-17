/*
* @@name:	array_sections.1c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
* @@version:	omp_4.0
*/
void foo ()
{
   int A[30];
#pragma omp target data map( A[0:4] )
{
   /* Cannot map distinct parts of the same array */
   #pragma omp target map( A[7:20] )
   {
      A[2] = 0;
   }
}
}
