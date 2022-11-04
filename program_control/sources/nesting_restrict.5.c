/*
* @@name:	nesting_restrict.5
* @@type:	C
* @@operation:	compile
* @@expect:	ct-error
* @@version:	pre_omp_3.0
*/
void work(int i, int j) {}
void wrong5(int n)
{
  #pragma omp parallel
  {
    #pragma omp critical
    {
       work(n, 0);
/* incorrect nesting of barrier region in a critical region */
       #pragma omp barrier
       work(n, 1);
    }
  }
}
