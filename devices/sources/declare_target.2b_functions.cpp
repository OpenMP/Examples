/*
* @@name:       declare_target.2b_functions
* @@type:       C++
* @@compilable: yes
* @@linkable:   no
* @@expect:     failure
* @@version:    omp_5.1
*/
#include "classes.hpp"
int XOR1::foo() { return a^0x01;}
