/*
* @@name:	nesting_restrict.1c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
*/

void work(int i, int j) {}

void wrong1(int n)
{

  #pragma omp parallel default(shared)
  {
    int i, j;
    #pragma omp for
    for (i=0; i<n; i++) {
       /* incorrect nesting of loop regions */
       #pragma omp for
         for (j=0; j<n; j++)
           work(i, j);
    }
  }

}
