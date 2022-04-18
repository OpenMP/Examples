/*
* @@name:	threadprivate.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
int counter = 0;
#pragma omp threadprivate(counter)

int increment_counter()
{
  counter++;
  return(counter);
}
