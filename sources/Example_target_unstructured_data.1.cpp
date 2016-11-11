/*
* @@name:        target-unstructured-data.1.cpp
* @@type:        C++
* @@compilable:  yes
* @@linkable:    no
* @@expect:      success
*/
class Matrix
{

  Matrix(int n) {
    len = n;
    v = new double[len];
    #pragma omp target enter data map(alloc:v[0:len])
  }

  ~Matrix() {
    // NOTE: delete map type should be used, since the corresponding 
    // host data will cease to exist after the deconstructor is called.

    #pragma omp target exit data map(delete:v[0:len])
    delete[] v;
  }

  private:
  double* v;
  int len;

};
