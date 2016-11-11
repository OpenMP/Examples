! @@name:	linear_in_loop.1f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
program linear_loop
   implicit none
   integer, parameter :: N = 100
   real :: a(N), b(N/2)
   integer :: i, j

   do i = 1, N
      a(i) = i
   end do

   j = 0
   !$omp parallel
   !$omp do linear(j:1)
   do i = 1, N, 2
      j = j + 1
      b(j) = a(i) * 2.0
   end do
   !$omp end parallel

   print *, j, b(1), b(j)
   ! print out: 50 2.0 198.0

end program
