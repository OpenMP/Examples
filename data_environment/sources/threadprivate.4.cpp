/*
* @@name:	threadprivate.4
* @@type:	C++
* @@operation:	view
* @@expect:	unspecified
* @@version:	pre_omp_3.0
*/
struct T { T (); T (int); ~T (); int t; };
int f();
static T t1;
#pragma omp threadprivate(t1)
static T t2( 23 );
#pragma omp threadprivate(t2)
static T t3 = f();
#pragma omp threadprivate(t3)
