/*
* @@name:	atomic.4
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.0
*/
#include <omp.h>

void calc_val(float *val);

void boxster(float *box_totals, float *vals, int *box, int N)
{
    #pragma omp parallel for
    for(int idx=0; idx<=N; idx++)
    {
       calc_val(&vals[idx]);
       #pragma omp atomic hint(omp_sync_hint_uncontended)
       box_totals[ box[idx] ] = box_totals[ box[idx] ] + vals[idx];
    }
}
