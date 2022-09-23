/*
* @@name:	threadprivate.5
* @@type:	C++
* @@compilable:	no
* @@linkable:	no
* @@expect:	nothing
*/
class T {
 public:
  static int i;
#pragma omp threadprivate(i)
};
