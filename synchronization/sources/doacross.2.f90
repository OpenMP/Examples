! @@name:	doacross.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.2
subroutine work( N, M, A, B, C )
  integer :: N, M, i, j
  real, dimension(M,N) :: A, B, C
  real, external :: foo, bar, baz

  !$omp do ordered(2)
  do j=2, N
    do i=2, M
      A(i,j) = foo(i, j)

    !$omp ordered doacross(sink: j-1,i) doacross(sink: j,i-1)
      B(i,j) = bar(A(i,j), B(i-1,j), B(i,j-1))
    !$omp ordered doacross(source:)

      C(i,j) = baz(B(i,j))
    end do
  end do
end subroutine
