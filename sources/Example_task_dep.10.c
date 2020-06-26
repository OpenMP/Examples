/*
* @@name:	task_dep.7c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
* @@version:	omp_5.0
*/
extern int longTaskA(), shortTaskB();
extern int shortTaskAC(int,int), longTaskBC(int,int);
void foo (void)
{
  int a, b, c;
  c = 0;
  #pragma omp parallel
  #pragma omp single
  {
     #pragma omp task depend(out: a)
        a = longTaskA();
     #pragma omp task depend(out: b)
        b = shortTaskB();
     #pragma omp task depend(in: a) depend(mutexinoutset: c)
        c = shortTaskAC(a,c);
     #pragma omp task depend(in: b) depend(mutexinoutset: c)
        c = longTaskBC(b,c);
  }
}
