/*
* @@name:	tasking.9c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	rt-error
* @@version:	omp_3.0
*/
void work()
{
   #pragma omp task
   { //Task 1
       #pragma omp task
       { //Task 2
            #pragma omp critical //Critical region 1
            {/*do work here */ }
       }
       #pragma omp critical //Critical Region 2
       {
           //Capture data for the following task
           #pragma omp task
           { /* do work here */ } //Task 3
       }
   }
}
