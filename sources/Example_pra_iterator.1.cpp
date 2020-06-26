/*
* @@name:	pra_iterator.1c
* @@type:	C++
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.0
*/
#include <vector>
void iterator_example()
{
  std::vector<int> vec(23);
  std::vector<int>::iterator it;
#pragma omp parallel for default(none) shared(vec)
  for (it = vec.begin(); it < vec.end(); it++)
  {
    // do work with *it //
  }
}
