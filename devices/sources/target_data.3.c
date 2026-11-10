/*
* @@name:	target_data.3
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_4.5
*/
#include <math.h>
#define COLS 100

void gramSchmidt(float Q[][COLS], const int rows)
{
    int cols = COLS;
    #pragma omp target data map(Q[0:rows][0:cols])
    for(int k=0; k < cols; k++)
    {
        double tmp = 0.0;

        // tmp is explicitly mapped so that its modified value
        // is exposed outside the target construct, as it would
        // otherwise be firstprivate and the reduction result
        // would be discarded.
        #pragma omp target map(tofrom: tmp)
        #pragma omp parallel for reduction(+:tmp)
        for(int i=0; i < rows; i++)
            tmp += (Q[i][k] * Q[i][k]);

        tmp = 1/sqrt(tmp);

        #pragma omp target
        #pragma omp parallel for
        for(int i=0; i < rows; i++)
            Q[i][k] *= tmp;
    }
}