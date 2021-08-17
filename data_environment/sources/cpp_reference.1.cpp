/*
* @@name:	cpp_reference.1c
* @@type:	C++
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.5
*/

void task_body (int &);
void gen_task (int &x) { // on orphaned task construct reference argument
  #pragma omp task // x is implicitly determined firstprivate(x)
  task_body (x);
}
void test (int &y, int &z) {
  #pragma omp parallel private(y)
  {
    y = z + 2;
    gen_task (y); // no matter if the argument is determined private
    gen_task (z); // or shared in the enclosing context.

    y++;          // each thread has its own int object y refers to
    gen_task (y);
  }
}

