/*
* @@name:	ordered.2
* @@type:	C
* @@operation:	compile
* @@expect:	ct-error
* @@version:	pre_omp_3.0
*/
void work(int i) {}

void ordered_wrong(int n)
{
  int i;
  #pragma omp for ordered
  for (i=0; i<n; i++) {
/* incorrect because an iteration may not execute more than one
   ordered region */
    #pragma omp ordered
      work(i);
    #pragma omp ordered
      work(i+1);
  }
}
