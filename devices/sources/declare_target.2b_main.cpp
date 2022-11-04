/*
* @@name:	declare_target.2b_main
* @@type:	C++
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.1
* @@depend:	declare_target.2b_classes.hpp
*/
#include <iostream>
using namespace std;

#include "declare_target.2b_classes.hpp"

int main (){

   XOR1 my_XOR1(3);
   int res1;

   #pragma omp target map(from: res1)
   res1=my_XOR1.foo();

   cout << res1 << endl;  // OUT1: 2
}
