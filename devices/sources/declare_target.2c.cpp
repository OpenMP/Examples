/*
* @@name:	declare_target.2c
* @@type:	C++
* @@operation:	compile
* @@expect:	ct-error
* @@version:	omp_5.1
*/
struct typeX
{
   int a;
};
class typeY
{
   int a;
  public:
   int foo() { return a^0x01;}
};

#pragma omp begin declare target
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
