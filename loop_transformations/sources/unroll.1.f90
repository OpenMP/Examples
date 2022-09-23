! @@name:       unroll.1
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.1
subroutine unroll(A, n)
   implicit none
   integer          :: i,n
   double precision :: A(n)

   !$omp unroll
   do i = 1,n
      A(i) = 0.0d0
   end do
end subroutine

subroutine unroll_full(A)
   implicit none
   integer :: i
   double precision :: A(*)

   !$omp unroll full
   do i = 1,4
      A(i) = 0.0d0
   end do
end subroutine

subroutine unroll_full_equivalent(A)
   implicit none
   double precision :: A(*)

   A(1) = 0.0d0
   A(2) = 0.0d0
   A(3) = 0.0d0
   A(4) = 0.0d0
end subroutine
