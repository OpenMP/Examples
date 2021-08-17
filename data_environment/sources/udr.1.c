/*
* @@name:	udr.1.c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
#include <limits.h>

struct point {
  int x;
  int y;
};

void minproc ( struct point *out, struct point *in )
{
  if ( in->x < out->x ) out->x = in->x;
  if ( in->y < out->y ) out->y = in->y;
}

void maxproc ( struct point *out, struct point *in )
{
  if ( in->x > out->x ) out->x = in->x;
  if ( in->y > out->y ) out->y = in->y;
}

#pragma omp declare reduction(min : struct point : \
        minproc(&omp_out, &omp_in)) \
	initializer( omp_priv = { INT_MAX, INT_MAX } )

#pragma omp declare reduction(max : struct point : \
        maxproc(&omp_out, &omp_in)) \
	initializer( omp_priv = { 0, 0 } )

void find_enclosing_rectangle ( int n, struct point points[] )
{
  struct point minp = { INT_MAX, INT_MAX }, maxp = {0,0};
  int i;

#pragma omp parallel for reduction(min:minp) reduction(max:maxp)
  for ( i = 0; i < n; i++ ) {
     minproc(&minp, &points[i]);
     maxproc(&maxp, &points[i]);
  }
  printf("min = (%d, %d)\n", minp.x, minp.y);
  printf("max = (%d, %d)\n", maxp.x, maxp.y);
}
