/*
* @@name:	declare_target.2b_functions
* @@type:	C++
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
* @@depend:	declare_target.2b_classes.hpp
*/
#include "declare_target.2b_classes.hpp"
int XOR1::foo() { return a^0x01;}
