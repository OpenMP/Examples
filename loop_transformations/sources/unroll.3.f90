! @@name:       unroll.3
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.1
subroutine unroll3_partial(A)
   implicit none
   double precision :: A(*)
   integer :: i

   !$omp unroll partial(4)
   do i = 1,128
      A(i) = 0
   end do
end subroutine

subroutine unroll3_partial_equivalent(A)
   implicit none
   double precision :: A(*)
   integer :: i_iv

   do i_iv = 0, 31
      A(i_iv * 4 + 1) = 0
      A(i_iv * 4 + 2) = 0
      A(i_iv * 4 + 3) = 0
      A(i_iv * 4 + 4) = 0
  end do
end subroutine

subroutine unroll3_partial_nofactor(A)
   implicit none
   double precision :: A(*)
   integer :: i

   !$omp unroll partial
   do i = 1, 128
      A(i) = 0
   end do
end subroutine
