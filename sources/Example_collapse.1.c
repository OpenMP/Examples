/*
* @@name:	collapse.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.0
*/

void bar(float *a, int i, int j, int k);

int kl, ku, ks, jl, ju, js, il, iu,is;

void sub(float *a)
{
    int i, j, k;

    #pragma omp for collapse(2) private(i, k, j)
    for (k=kl; k<=ku; k+=ks)
       for (j=jl; j<=ju; j+=js)
          for (i=il; i<=iu; i+=is)
             bar(a,i,j,k);
}
