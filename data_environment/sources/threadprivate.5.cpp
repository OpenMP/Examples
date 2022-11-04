/*
* @@name:	threadprivate.5
* @@type:	C++
* @@operation:	view
* @@expect:	unspecified
* @@version:	pre_omp_3.0
*/
class T {
 public:
  static int i;
#pragma omp threadprivate(i)
};
