! @@name:       apply_syntax.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine construct_unroll(A)
   implicit none
   integer :: i
   double precision :: A(0:99)

   !$omp unroll
   !$omp tile sizes(4)
   do i = 0, 99
      A(i) = A(i) + 1
   end do
end subroutine

subroutine apply_unroll(A)
   implicit none
   integer :: i
   double precision :: A(0:99)

   !$omp tile sizes(4) apply(grid: unroll)
   do i = 0, 99
      A(i) = A(i) + 1
   end do
end subroutine
