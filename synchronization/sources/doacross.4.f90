! @@name:	doacross.4
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:    omp_5.2

subroutine work( N, M, A, B, C )
  integer :: N, M
  real(8), dimension(M, N) :: A, B, C
  real(8), external :: foo
  integer :: i, j
  real(8) :: alpha = 1.2

  !$omp do collapse(2) ordered(2)
  do j=2, N-1
    do i=2, M-1
      A(i,j) = foo(i, j)
    !$omp ordered doacross(source:)

      B(i,j) = alpha * A(i,j)

    !$omp ordered doacross(sink: j,i-1) doacross(sink: j-1,i)
      C(i,j) = 0.2 * (A(i-1,j) + A(i+1,j) +  &
               A(i,j-1) + A(i,j+1) + A(i,j))
    end do
  end do
end subroutine
