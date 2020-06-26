! @@name:	lastprivate.2f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version: omp_5.0
function condlastprivate(a, n) result(x)
   implicit none
   real a(*), x
   integer n, k

   x = 0.0

   !$omp parallel do simd lastprivate(conditional: x)
   do k = 1, n
      if (a(k) < 108.5 .or. a(k) > 208.5) then
         x = sin(a(k))
      endif
   end do

end function condlastprivate
