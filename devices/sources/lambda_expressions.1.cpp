/*
* @@name:	lambda_expressions.1
* @@type:	C++
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/
#include <iostream>
using namespace std;

struct S { int a; int b; };

int main()
{

// CASE 1 Lambda defined in target region

   S s = S {0,1};

   #pragma omp target
   {   
      auto lambda1 = [&s]() { s.a = s.b * 2; };  
      s.b += 2;
      lambda1(); // s.a = 3 * 2 
   }   
   cout << s.a << " " << s.b << endl; //OUT 6 3

// CASE 2 Host defined lambda, Capture pointer to s

   s = {0,1};
   S *sp = &s;
   auto lambda2 = [sp]() {sp->a = sp->b * 2; };

   // closure object's sp attaches to corresponding s on target
   // construct
   #pragma omp target data map(sp[0])
   #pragma omp target
   {
      sp->b += 2;
      lambda2();
   }
   cout << s.a << " " << s.b << endl; //OUT 6 3

// CASE 3 Host defined lambda, Capture s by reference

   s = {0,1};
   auto lambda3 = [&s]() {s.a = s.b * 2; };

   // closure object's s refers to corresponding s in target
   // construct
   #pragma omp target data map(s)
   #pragma omp target
   {
      s.b += 2;
      lambda3();
   }
   cout << s.a << " " << s.b << endl; //OUT 6 3

// CASE 4 Host defined lambda, references static variable

   static S ss = {0,1};
   #pragma omp declare target enter(ss)
   auto lambda4 = [&]() {ss.a = ss.b * 2; };

   #pragma omp target map(always,from:ss)
   {
      ss.b += 2;
      lambda4();
   }
   cout << ss.a << " " << ss.b << endl; //OUT 6 3

   return 0;
}
