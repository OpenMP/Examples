/*
* @@name:       non-comm-reduce.1
* @@type:       C++   
* @@operation:  run 
* @@expect:     success
* @@version:    omp_6.1
*/
#include <vector>
#include <iostream>
#include <string.h>
#include <numeric>

// Matrix x Matrix multiplication -- Square Matrices (NxN)
#define N 2

template <typename T>
struct Matrix {
  T mat[N][N];

  Matrix(){
    memset(mat, 0, sizeof(mat[0][0])*N*N);
    for(int i=0; i<N; ++i) mat[i][i]=1;
  }
};

template <typename T>
bool operator==(const Matrix<T>& left, const Matrix<T>& right) {
  for (int i = 0; i < N; i++) 
    for (int j = 0; j < N; j++) 
      if(left.mat[i][j] != right.mat[i][j] ) return false;
  return true;
}     

template <typename T>
Matrix<T> operator*(const Matrix<T> &left,const Matrix<T> &right) {
  Matrix<T> result;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result.mat[i][j] = 0;
      for (int k = 0; k < N; k++)
        result.mat[i][j] += left.mat[i][k] * right.mat[k][j];
    }   
  }   
  return result;
}

template <typename InputIterator, typename Tp>
auto prod(InputIterator A, int nmats, Matrix<Tp> &init, bool do_par) {

  Matrix<Tp> PROD = init;   // Identity Matrix

  #pragma omp declare_reduction( *: Matrix<Tp> ) \
              combiner( omp_out = omp_out*omp_in ) \
              initializer( omp_priv = Matrix<Tp>{} )

  #pragma omp parallel for if(do_par) \
              reduction(noncommutative, *: PROD) num_threads(nmats/2)
  for (int i = 0; i < nmats; ++i) {
    PROD = PROD * A[i];
  }
  return PROD;
}

int main() {
  int nmats = 6;
  
  std::vector<Matrix<int>> A(nmats);  // array of input Matrices
  for (int i = 0; i < nmats; ++i)
    std::iota(&A[i].mat[0][0], &A[i].mat[0][0]+N*N, 1); // Fills 1,2,...

  Matrix<int> identity;  // initializes to unit Matrix

  auto parallel_prod = prod(A.cbegin(), nmats, identity, true );
  auto   serial_prod = prod(A.cbegin(), nmats, identity, false);

  if (parallel_prod == serial_prod) std::cout << "passed\n"; 
  else                              std::cout << "failed\n";

  return 0;
}
