/*
* @@name:	udr.6
* @@type:	C++
* @@operation:	view
* @@expect:	unspecified
* @@version:	omp_4.0
*/
#include <algorithm>
#include <list>
#include <vector>

#pragma omp declare reduction( + : std::vector<int> : \
     std::transform (omp_out.begin(), omp_out.end(),  \
		      omp_in.begin(), omp_in.end(),std::plus<int>()))

#pragma omp declare reduction( merge : std::vector<int> : \
     omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

#pragma omp declare reduction( merge : std::list<int> : \
     omp_out.merge(omp_in))
