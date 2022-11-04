/*
* @@name:	nesting_restrict.6
* @@type:	C
* @@operation:	compile
* @@expect:	ct-error
* @@version:	pre_omp_3.0
*/
void work(int i, int j) {}
void wrong6(int n)
{
  #pragma omp parallel
  {
    #pragma omp single
    {
      work(n, 0);
/* incorrect nesting of barrier region in a single region */
      #pragma omp barrier
      work(n, 1);
    }
  }
}
