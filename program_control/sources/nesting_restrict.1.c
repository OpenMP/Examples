/*
* @@name:	nesting_restrict.1
* @@type:	C
* @@operation:	compile
* @@expect:	ct-error
* @@version:	pre_omp_3.0
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
