/*
* @@name:	pra_iterator.1
* @@type:	C++
* @@operation:	compile
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
