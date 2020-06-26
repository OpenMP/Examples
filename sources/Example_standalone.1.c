/*
* @@name:	standalone.1c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	failure
* @@version:	omp_3.1
*/

void standalone_wrong()
{
  int a = 1;

	if (a != 0)
  #pragma omp flush(a)
/* incorrect as flush cannot be immediate substatement
   of if statement */

	if (a != 0)
  #pragma omp barrier
/* incorrect as barrier cannot be immediate substatement
   of if statement */

	if (a!=0)
  #pragma omp taskyield
/* incorrect as taskyield cannot be immediate substatement of if statement
*/

	if (a != 0)
  #pragma omp taskwait
/* incorrect as taskwait cannot be immediate substatement
   of if statement */

}
