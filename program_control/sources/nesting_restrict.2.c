/*
* @@name:	nesting_restrict.2
* @@type:	C
* @@operation:	compile
* @@expect:	unspecified
* @@version:	pre_omp_3.0
*/
void work(int i, int j) {}
void work1(int i, int n)
{
  int j;
/* incorrect nesting of loop regions */
  #pragma omp for
    for (j=0; j<n; j++)
      work(i, j);
}

void wrong2(int n)
{
  #pragma omp parallel default(shared)
  {
    int i;
    #pragma omp for
      for (i=0; i<n; i++)
         work1(i, n);
  }
}
