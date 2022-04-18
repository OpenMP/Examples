/*
* @@name:       declare_target.2a
* @@type:       C++
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.1
*/
#include <iostream>
using namespace std;

  #pragma omp begin declare target // declare target--class and function
  class XOR1
  {
      int a;
    public:
      XOR1(int arg): a(arg) {};
      int foo();
  }
  int XOR1::foo() { return a^0x01;}
  #pragma omp end declare target

  #pragma omp begin declare target // declare target--class, not function
  class XOR2
  {
      int a;
    public:
      XOR2(int arg): a(arg) {};
      int foo();
  };
  #pragma omp end declare target

  int XOR2::foo() { return a^0x01;} 

  class XOR3                // declare target--neither class nor function 
  {
      int a;
    public:
      XOR3(int arg): a(arg) {};
      int foo();
  };
  int XOR3::foo() { return a^0x01;}

int main (){

    XOR1 my_XOR1(3);
    XOR2 my_XOR2(3);
    XOR3 my_XOR3(3);
    int res1, res2, res3;

    #pragma omp target map(tofrom:res1)
    res1=my_XOR1.foo();

    #pragma omp target map(tofrom:res2)
    res2=my_XOR2.foo();

    #pragma omp target map(tofrom:res3)
    res3=my_XOR3.foo();

    cout << res1 << endl;  // OUT1: 2
    cout << res2 << endl;  // OUT2: 2
    cout << res3 << endl;  // OUT3: 2
}
