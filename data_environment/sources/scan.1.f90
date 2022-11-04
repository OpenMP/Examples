! @@name:	scan.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
program inclusive_scan
   implicit none
   integer, parameter :: n = 100
   integer a(n), b(n)
   integer x, k

   ! initialization
   x = 0
   do k = 1, n
      a(k) = k
   end do

   ! a(k) is included in the computation of producing results in b(k)
   !$omp parallel do simd reduction(inscan,+: x)
   do k = 1, n
      x = x + a(k)
      !$omp scan inclusive(x)
      b(k) = x
   end do

   print *,'x =', x, ', b(1:3) =', b(1:3)
   !           5050,            1  3  6

end program
