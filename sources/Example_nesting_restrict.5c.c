/*
* @@name:	nesting_restrict.5c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
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
