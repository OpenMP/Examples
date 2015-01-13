/*
* @@name:	declare_target.2c
* @@type:	C++
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
*/
struct typeX
{
   int a;
}
class typeY
{
   int foo() { return a^0x01;}
   int a;
}
#pragma omp declare target
struct typeX varX;  // ok
class typeY varY; // ok if varY.foo() not called on target device
#pragma omp end declare target
void foo()
{
   #pragma omp target
   {
      varX.a = 100; // ok
      varY.foo(); // error foo() is not available on a target device
   }
}
