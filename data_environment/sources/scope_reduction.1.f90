! @@name:	scope_reduction.1f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_5.1
subroutine do_work(n, a, s)
   implicit none
   integer n, i
   real a(*), s, loc_s
   integer, save :: nthrs

   loc_s = 0.0                ! local sum
   !$omp do
      do i = 1, n
         loc_s = loc_s + a(i)
      end do
   !$omp single
      s = 0.0                 ! total sum
      nthrs = 0
   !$omp end single
   !$omp scope reduction(+:s,nthrs)
      s = s + loc_s
      nthrs = nthrs + 1
   !$omp end scope
   !$omp masked
      print *, "total sum = ", s, ", nthrs = ", nthrs
   !$omp end masked
end subroutine

function work(n, a) result(s)
   implicit none
   integer n
   real a(*), s

   !$omp parallel
      call do_work(n, a, s)
   !$omp end parallel
end function
