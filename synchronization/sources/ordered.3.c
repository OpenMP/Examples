/*
* @@name:	ordered.3
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	pre_omp_3.0
*/
void work(int i) {}
void ordered_good(int n)
{
  int i;
#pragma omp for ordered
  for (i=0; i<n; i++) {
    if (i <= 10) {
      #pragma omp ordered
         work(i);
    }
    if (i > 10) {
      #pragma omp ordered
        work(i+1);
    }
  }
}
