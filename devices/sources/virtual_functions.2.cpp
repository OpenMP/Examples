/*
* @@name:	virtual_functions.2
* @@type:	C++
* @@operation:	link
* @@expect:	rt-error
* @@version:	omp_5.2
*/
#include <iostream>
#pragma omp requires unified_shared_memory

#pragma omp begin declare target
class A {  
  public:
   virtual void vf()  { std::cout << "In A\n"; }
};

class D: public A {
  public:
   void vf() override { std::cout << "In D\n"; }
};
#pragma omp end declare target

int main(){

   A *ap = nullptr;
   // Case 1
   #pragma omp target
   {
        ap = new D();
   }
   ap->vf();     // illegal
   #pragma omp target
   {
      delete ap;
   }

   // Case 2
   ap = new D();
   #pragma omp target  // No need for mapping with Unified Share Memory
   {
      ap->vf();  // ok
   }

   return 0;
}
