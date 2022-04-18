/*
* @@name:       requires.1
* @@type:       C++
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.0
*/

#include <iostream>
using namespace std;

#pragma omp requires unified_shared_memory

typedef struct mypoints
{
   double res;
   double data[500];
} mypoints_t;

void do_something_with_p(mypoints_t *p, int q);

int main()
{
  mypoints_t p;
  int q=0;

  #pragma omp target // no map clauses needed
  {                  // q is firstprivate
     q++;
     do_something_with_p(&p,q);
  }
  cout<< p.res << " " << q << endl;  // output 1 0
  return 0;
}
void do_something_with_p(mypoints_t *p, int q)
{
  p->res = q;
  for(int i=0;i<sizeof(p->data)/sizeof(double);i++)
      p->data[i]=q*i;
}
