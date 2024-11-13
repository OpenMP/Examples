! @@name:	taskloop_dep.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
subroutine process_work_b(n, B)
   implicit none
   integer :: n
   real :: B(n,*)
   integer :: i, j

   ! Dependences for taskloop iterations in multi-dimensional loop nest

   ! TL4 taskloop + collapse
   !$omp taskloop collapse(2) nogroup
   do j = 2, n
      do i = 2, n
         !$omp task_iteration depend(inout: B(i,j))  &
         !$omp&               depend(in: B(i-1,j), B(i,j-1))
         B(i,j) = B(i,j) + B(i,j) * B(i-1,j) * B(i,j-1)
      end do
   end do
   !$omp end taskloop

   ! T5 other task
   !$omp task depend(in: B(n,n))
   print *, "B(n,n) =", B(n,n)
   !$omp end task
end subroutine
