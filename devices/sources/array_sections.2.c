/*
* @@name:	array_sections.2
* @@type:	C
* @@operation:	compile
* @@expect:	ct-error
* @@version:	omp_4.0
*/
void foo ()
{
   int A[30], *p;
#pragma omp target data map( A[0:4] )
{
   p = &A[0];
   /* invalid because p[3] and A[3] are the same
    * location on the host but the array section
    * specified via p[...] is not a subset of A[0:4] */
   #pragma omp target map( p[3:20] )
   {
      A[2] = 0;
      p[8] = 0;
   }
}
}
