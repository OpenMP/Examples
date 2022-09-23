! @@name:       scan.2
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version: omp_5.0
program exclusive_scan
   implicit none
   integer, parameter :: n = 100
   integer a(n), b(n)
   integer x, k

   ! initialization
   x = 0
   do k = 1, n
      a(k) = k
   end do

   ! a(k) is not included in the computation of producing results in b(k)
   !$omp parallel do simd reduction(inscan,+: x)
   do k = 1, n
      b(k) = x
      !$omp scan exclusive(x)
      x = x + a(k)
   end do

   print *,'x =', x, ', b(1:3) =', b(1:3)
   !           5050,            0  1  3

end program
