/*
* @@name:	virtual_functions.1
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

   // Section 1 --------------------------------------------------------
   D d;               // D derives from A, and A::vf() is virtual
   A &ar = d;         // reference to Derived object d

   #pragma omp target // implicit map of ar is illegal here
   {
      ar.vf();        // unspecified whether A::vf() or D::vf() is called
   }
   
   A *ap = &d;        // pointer to derived object d
   #pragma omp target // No need for mapping with Unified Share Memory
   {                  // implicit ap[:0] map is fine 
      ap->vf();       // calls D::vf()
   }

   // Section 2 --------------------------------------------------------
   ap = nullptr;
   #pragma omp target map(ap)
   {
        ap = new A();
   }
   
   ap->vf();     // illegal

   #pragma omp target
   {
      delete ap;
   }
   ap = new A();
   #pragma omp target  // No need for mapping with Unified Share Memory
   {
      ap->vf();  // ok
   }

   return 0;
}
