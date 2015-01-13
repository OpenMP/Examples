/*
* @@name:	nesting_restrict.4c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
*/
void work(int i, int j) {}
void wrong4(int n)
{

  #pragma omp parallel default(shared)
  {
    int i;
    #pragma omp for
      for (i=0; i<n; i++) {
        work(i, 0);
/* incorrect nesting of barrier region in a loop region */
        #pragma omp barrier
        work(i, 1);
      }
  }
}
