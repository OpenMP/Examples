! @@name:       unroll.2
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.1
subroutine illegal_2a(A)
   implicit none
   double precision :: A(*)
   integer :: i

   !$omp do
   !$omp unroll full  !! ERROR: No loop left after full unrolling
   do i = 1,12
      A(i) = 0.0d0
   end do
end subroutine

subroutine illegal_2b(A)
   implicit none
   double precision :: A(*)
   integer :: i

   !! Loop might be fully unrolled (or a partially unrolled loop 
   !! replacement).  Hence, no canonical do-loop will exist, 
   !! resulting in non-compliant code.
   !! Implementations may suggest to adding a "partial" clause.

   !$omp do           !!        Requires a canonical loop
   !$omp unroll       !! ERROR: may result in non-compliant code
   do i = 1,12
      A(i) = 0.0d0
   end do
end subroutine

subroutine illegal_2c(n, A)
   implicit none
   integer          :: i,n
   double precision :: A(*)

   !$omp unroll full  !! Full unroll requires constant iteration count
   do i = 1,n
      A(i) = 0.0d0
   end do
end subroutine
