! @@name:	scan.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
program inclusive_scan
   implicit none
   integer, parameter :: n = 100
   integer c(n), tmp
   integer x, k

   ! initialization
   x = 0
   do k = 1, n
      c(k) = k
   end do

   ! c(k) is used for both input and output of scan results
   !$omp parallel do simd reduction(inscan,+: x) private(tmp)
   do k = 1, n
      ! initialization phase
      tmp = c(k)
      !$omp scan init_complete

      ! scan (output) phase - cannot use tmp here
      c(k) = x

      !$omp scan exclusive(x)

      ! input phase - can use tmp here
      x = x + tmp
   end do

   print *,'x =', x, ', c(1:3) =', c(1:3)
   !           5050,            0  1  3

end program
