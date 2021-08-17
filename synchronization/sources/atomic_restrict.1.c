/*
* @@name:	atomic_restrict.1c
* @@type:	C
* @@compilable:	maybe
* @@linkable:	no
* @@expect:	failure
* @@version:	omp_3.1
*/
void atomic_wrong ()
{
 union {int n; float x;} u;

#pragma omp parallel
  {
#pragma omp atomic update
    u.n++;

#pragma omp atomic update
    u.x += 1.0;

/* Incorrect because the atomic constructs reference the same location
   through incompatible types */
  }
}
