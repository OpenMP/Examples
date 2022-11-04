/*
* @@name:	tasking.8
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_3.0
*/
int tp;
#pragma omp threadprivate(tp)
int var;
void work()
{
#pragma omp parallel
    {
        /* do work here */
#pragma omp task
        {
            tp++;
            /* do work here */
#pragma omp task
            {
                /* do work here but don't modify tp */
            }
            var = tp; //Value does not change after write above
        }
    }
}
