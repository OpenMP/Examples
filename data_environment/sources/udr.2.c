/*
* @@name:	udr.2
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>
#include <limits.h>

struct point {
  int x;
  int y;
};

#pragma omp declare reduction(min : struct point)  \
      combiner( omp_out.x = omp_in.x > omp_out.x ? omp_out.x : omp_in.x,  \
                omp_out.y = omp_in.y > omp_out.y ? omp_out.y : omp_in.y ) \
      initializer( omp_priv = { INT_MAX, INT_MAX } )

#pragma omp declare reduction(max : struct point)  \
      combiner( omp_out.x = omp_in.x < omp_out.x ? omp_out.x : omp_in.x,  \
                omp_out.y = omp_in.y < omp_out.y ? omp_out.y : omp_in.y ) \
      initializer( omp_priv = { 0, 0 } )

void find_enclosing_rectangle ( int n, struct point points[] )
{
  struct point minp = { INT_MAX, INT_MAX }, maxp = {0,0};
  int i;

#pragma omp parallel for reduction(min:minp) reduction(max:maxp)
  for ( i = 0; i < n; i++ ) {
    if ( points[i].x < minp.x ) minp.x = points[i].x;
    if ( points[i].y < minp.y ) minp.y = points[i].y;
    if ( points[i].x > maxp.x ) maxp.x = points[i].x;
    if ( points[i].y > maxp.y ) maxp.y = points[i].y;
  }
  printf("min = (%d, %d)\n", minp.x, minp.y);
  printf("max = (%d, %d)\n", maxp.x, maxp.y);
}
