/*
* @@name:	threadprivate.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
int increment_counter_2()
{
  static int counter = 0;
  #pragma omp threadprivate(counter)
  counter++;
  return(counter);
}
