/*
* @@name:	nesting_restrict.6c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
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
