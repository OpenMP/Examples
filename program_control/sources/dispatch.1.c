/*
* @@name:	dispatch.1
* @@type:   	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>
 
int foo_sub;

void foo_variant1()
{ printf("in foo_variant1\n"); }
 
void foo_variant2()
{ printf("in foo_variant2\n"); }
 
#pragma omp declare variant(foo_variant1) \
                    match(user={condition(foo_sub)}) 
#pragma omp declare variant(foo_variant2) \
                    match(construct={dispatch},user={condition(foo_sub)}) 
void foo()
{ printf("in foo\n"); }

int main()
{
   // Case 1
   foo_sub = 1;
   foo();        // "in foo_variant1"

   // Case 2
   foo_sub = 0;
   foo();        // "in foo"

   // Dispatch Cases

   // Case 3
   foo_sub = 0;
   #pragma omp dispatch
   foo();        // "in foo"

   // Case 4
   foo_sub = 1;  
   #pragma omp dispatch
   foo();        // "in foo_variant2"
                 // see discussion for OpenMP 5.1/5.2
 
   // Case 5
   foo_sub = 1;  
   #pragma omp dispatch novariants(1)
   foo();        // "in foo"

   // Case 6
   foo_sub = 1;  
   #pragma omp dispatch nocontext(1)
   foo();        // "in foo_variant1"

   return 0;
}
