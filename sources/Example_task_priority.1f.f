! @@name:	task_priority.1f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
subroutine compute_matrix(matrix, M, N)
   implicit none
   integer :: M, N
   real :: matrix(M, N)
   integer :: i
   interface
      subroutine compute_array(node, M)
      implicit none
      integer :: M
      real :: node(M)
      end subroutine
   end interface
   !$omp parallel private(i)
   !$omp single
   do i=1,N
      !$omp task priority(i)
      call compute_array(matrix(:, i), N)
      !$omp end task
   enddo
   !$omp end single
   !$omp end parallel
end subroutine compute_matrix
