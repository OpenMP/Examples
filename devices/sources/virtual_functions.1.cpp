/*
* @@name:	virtual_functions.1
* @@type:	C++
* @@operation:	link
* @@expect:	rt-error
* @@version:	omp_5.2
*/
#include <iostream>

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

   D d;               // D derives from A, and vf() is virtual

 #pragma omp target data map(d)
 {
    // Case 1
    A *ap = &d;        // pointer to derived object d
    #pragma omp target // ap is firstprivate
    {
       ap->vf();       // calls D::vf()
    }

    // Case 2
    A &ar = d;         // reference to Derived object d
    #pragma omp target // ar is implicitly mapped
    {
       ar.vf();        // unspecified behavior
    }
 }

   return 0;
}
