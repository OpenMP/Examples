! @@name:	task_priority.1
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:    omp_4.5
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
      call compute_array(matrix(:, i), M)
      !$omp end task
   enddo
   !$omp end single
   !$omp end parallel
end subroutine compute_matrix
