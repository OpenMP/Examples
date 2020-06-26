/*
* @@name:	task_dep.6c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.0
*/
#include <stdio.h>
int main()
{
   int a, b, c, d;
   #pragma omp parallel
   #pragma omp single
   {
      #pragma omp task depend(out: c)
         c = 1;   /* Task T1 */
      #pragma omp task depend(out: a)
         a = 2;   /* Task T2 */
      #pragma omp task depend(out: b)
         b = 3;   /* Task T3 */
      #pragma omp task depend(in: a) depend(mutexinoutset: c)
         c += a;  /* Task T4 */
      #pragma omp task depend(in: b) depend(mutexinoutset: c)
         c += b;  /* Task T5 */ 
      #pragma omp task depend(in: c)
         d = c;   /* Task T6 */
   }
   printf("%d\n", d);
   return 0;
}
