! @@name:	collapse.4f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_5.0
module calc_m
  interface
  subroutine calc_a(n, m, a)
  integer n, m
  real a(n+2,m)
  ! routine to calculate a
  ! For variable a(*,j):
  ! a(1,j),...,a(n,j)  contains the n samples
  ! a(n+1,j)           contains the sample mean
  ! a(n+2,j)           contains the standard deviation 
  end subroutine
  end interface
end module

program main
  use calc_m
  integer, parameter :: N=20, M=10
  real a(N+2,M), b(M,M)
  real temp
  integer i, j, k

  call calc_a(N,M,a)

  !$omp parallel do collapse(2) private(k,temp)
  do i = 1, M
     do j = i, M 
        temp = 0.0
        do k = 1, N
           temp = temp + (a(k,i)-a(N+1,i))*(a(k,j)-a(N+1,j))
        end do

        b(i,j) = temp / (a(N+2,i) * a(N+2,j) * (N - 1))
        b(j,i) = b(i,j)
     end do
  end do

  print *,"b(1,1) = ",b(1,1),", b(M,M) = ",b(M,M)

end program
