/*
* @@name        declare_target.2b
* @@type:       C++
* @@compilable  yes
* @@linkable:   no
* @@expect:     failure
* @@version:    omp_5.1
*/
#include <iostream>
using namespace std;

#include "classes.hpp"

int main (){

   XOR1 my_XOR1(3);
   int res1;

   #pragma omp target map(from: res1)
   res1=my_XOR1.foo();

   cout << res1 << endl;  // OUT1: 2
}
