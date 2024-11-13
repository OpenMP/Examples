! @@name:       apply_nested.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine nested_apply(A)
   implicit none
   double precision :: A(0:99)
   integer          :: i

   !$omp tile sizes(10) apply(intratile: unroll partial(2) apply(reverse))
   do i = 0, 99
      A(i) = A(i) + 1 
   enddo
end subroutine
