/*
* @@name:	induction.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>
#include <math.h>

void comp_poly(int N, double x, double c[]) {
   // x:    input: value of x for which to eval the polynomial
   // c[N]: input: the coefficients
   double x0 = 1.0;       // initial value x^0 == 1
   double xi;             // x^i
   double result;         // accumulator for the result

   // Case 1: induction clause
   xi = x0;
   result = 0.0;
   #pragma omp parallel for reduction(+: result) induction(step(x),*: xi)
   for (int i = 0; i < N; i++) {
      result += c[i] * xi;
      xi *= x;
   }
   printf("C1: result = %f, xn = %f\n", result, xi);

   // Case 2: inscan reduction
   xi = x0;
   result = 0.0;
   #pragma omp parallel for reduction(+: result) reduction(inscan,*: xi)
   for (int i = 0; i < N; i++) {
      result += c[i] * xi;
      #pragma omp scan exclusive(xi)
      xi *= x;
   }
   printf("C2: result = %f, xn = %f\n", result, xi);

   // Case 3: closed form
   result = 0.0;
   #pragma omp parallel for reduction(+: result) lastprivate(xi)
   for (int i = 0; i < N; i++) {
      xi = x0 * pow(x, i);     // induction operation in closed form
      result += c[i] * xi;
      xi *= x;
   }
   printf("C3: result = %f, xn = %f\n", result, xi);
}
