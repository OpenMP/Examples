/*
* @@name:	unroll.4
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:    omp_5.1
*/
void unroll_partial_remainder(int n, int A[])
{
    #pragma omp parallel for
    #pragma omp unroll partial(4)
    for (int i = 0; i < n; ++i)
        A[i] = i;
}

void unroll_partial_remainder_option1(int n, int A[])
{
    #pragma omp parallel for
    for (int i_iv = 0; i_iv < (n+3)/4; ++i_iv) {
                              A[i_iv * 4 + 0] = i_iv * 4 + 0;
        if (i_iv * 4 + 1 < n) A[i_iv * 4 + 1] = i_iv * 4 + 1;
        if (i_iv * 4 + 2 < n) A[i_iv * 4 + 2] = i_iv * 4 + 2;
        if (i_iv * 4 + 3 < n) A[i_iv * 4 + 3] = i_iv * 4 + 3;
   }
}

void unroll_partial_remainder_option2(int n, int A[])
{
    #pragma omp parallel for
    for (int i_iv = 0; i_iv < (n+3)/4; ++i_iv) {
        if (i_iv < n/4) {
            A[i_iv * 4 + 0] = i_iv * 4 + 0;
            A[i_iv * 4 + 1] = i_iv * 4 + 1;
            A[i_iv * 4 + 2] = i_iv * 4 + 2;
            A[i_iv * 4 + 3] = i_iv * 4 + 3;
        } else {
            // remainder loop
            for (int i_rem = i_iv*4; i_rem < n; ++i_rem)
                A[i_rem] = i_rem;
        }
    }
}

void unroll_partial_remainder_option3(int n, int A[])
{
    // main loop
    #pragma omp parallel for
    for (int i_iv = 0; i_iv < n/4; ++i_iv) {
        A[i_iv * 4 + 0] = i_iv * 4 + 0;
        A[i_iv * 4 + 1] = i_iv * 4 + 1;
        A[i_iv * 4 + 2] = i_iv * 4 + 2;
        A[i_iv * 4 + 3] = i_iv * 4 + 3;
    }

    // remainder loop
    #pragma omp parallel for
    for (int i_rem = (n/4)*4; i_rem < n; ++i_rem)
        A[i_rem] = i_rem;
}

#include <stdio.h>
#define NT 12

int main(){
int error=0, A[NT],C[NT];
for(int i = 0; i<NT; i++){ A[i]=0; C[i]=i; }

    for(int i = 0; i<NT; i++) A[i]=0.0;
    unroll_partial_remainder(NT,A);
    for(int i = 0; i<NT; i++) if(A[i] != C[i]) error=1;

    for(int i = 0; i<NT; i++) A[i]=0.0;
    unroll_partial_remainder_option1(NT,A);
    for(int i = 0; i<NT; i++) if(A[i] != C[i]) error=1;

    for(int i = 0; i<NT; i++) A[i]=0.0;
    unroll_partial_remainder_option2(NT,A);
    for(int i = 0; i<NT; i++) if(A[i] != C[i]) error=1;

    for(int i = 0; i<NT; i++) A[i]=0.0;
    unroll_partial_remainder_option3(NT,A);
    for(int i = 0; i<NT; i++) if(A[i] != C[i]) error=1;

  if(!error) printf("OUT: Passed\n");
  if( error) printf("OUT: Failed\n");
}
