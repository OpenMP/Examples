/*
* @@name:	threadprivate.4c
* @@type:	C++
* @@compilable:	no
* @@linkable:	no
* @@expect:	nothing
*/
struct T { T (); T (int); ~T (); int t; };
int f();
static T t1;
#pragma omp threadprivate(t1)
static T t2( 23 );
#pragma omp threadprivate(t2)
static T t3 = f();
#pragma omp threadprivate(t3)
