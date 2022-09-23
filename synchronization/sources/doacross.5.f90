! @@name:	doacross.5
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:    omp_5.2
subroutine work( N, A, B, C )
  integer :: N, i
  real, dimension(N) :: A, B, C
  real, external :: foo, bar, baz

  !$omp do ordered(1)
  do i=2, N
    A(i) = foo(i)

  !$omp ordered depend(sink: i-1)
    B(i) = bar(A(i), B(i-1))
  !$omp ordered depend(source)

    C(i) = baz(B(i))
  end do
end subroutine
