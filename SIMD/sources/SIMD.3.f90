! @@name:	SIMD.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
subroutine work( a, b, n, sum )
   implicit none
   integer :: i, n
   double precision :: a(n), b(n), sum, tmp

   sum = 0.0d0
   !$omp simd private(tmp) reduction(+:sum)
   do i = 1,n
      tmp = a(i) + b(i)
      sum = sum + tmp
   end do

end subroutine work
