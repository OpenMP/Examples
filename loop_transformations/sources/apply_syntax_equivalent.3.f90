! @@name:	apply_syntax_equivalent.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
subroutine apply_complexarg_equivalent1(A)
   implicit none
   double precision :: A(100,100)
   integer :: i, j

   !$omp tile sizes(4,5)                 &
   !$omp&   apply(grid: reverse,nothing) &
   !$omp&   apply(intratile: nothing,unroll)
   do i = 1, 100
      do j = 1, 100
         A(j,i) = A(j,i) + 1
      end do
   end do
end subroutine

subroutine apply_complexarg_equivalent2(A)
   implicit none
   double precision :: A(100,100)
   integer :: i, j, i1, j1

   !$omp reverse
   do i1 = 1, 100, 4          ! grid loop 1
      do j1 = 1, 100, 5       ! grid loop 2
         do i = i1, i1+3      ! tile loop 1
            !$omp unroll
            do j = j1, j1+4   ! tile loop 2
               A(j,i) = A(j,i) + 1
            end do
         end do
      end do
   end do
end subroutine
