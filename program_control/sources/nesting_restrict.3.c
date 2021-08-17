/*
* @@name:	nesting_restrict.3c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
*/
void work(int i, int j) {}
void wrong3(int n)
{
  #pragma omp parallel default(shared)
  {
    int i;
    #pragma omp for
      for (i=0; i<n; i++) {
/* incorrect nesting of regions */
        #pragma omp single
          work(i, 0);
      }
  }
}
