! @@name:	task_dep.14
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
program main
  use omp_lib
  integer, parameter :: N_ROWS = 20
  integer, parameter :: N_COLS = 20
  integer, parameter :: NUM_VS = 5
  integer, parameter :: ROWS_PER_TASK = 5
  integer :: h
  integer :: M(0:N_ROWS*N_COLS-1), v(0:N_COLS-1,0:NUM_VS-1)
  integer :: check_value

  M(:) = 1
  v(:,:) = 2

  !$omp parallel single
    do h = 0, NUM_VS-1
      ! Generate transparent task to establish dependences
      ! between child tasks that don't share the same parent.
      !$omp task depend(inout:M) transparent(omp_impex)
        call my_func(M, v(:,h))
      !$omp end task
    end do
  !$omp end parallel single

  check_value = 2**NUM_VS
  if (any(M /= check_value)) error stop

contains
  subroutine my_func(M, v)
    integer :: M(0:), v(0:)
    integer :: i,j,k

    do i = 0, N_ROWS-1, ROWS_PER_TASK
      ! This task is dependency-ordered with respect to the corresponding
      ! task in iteration i generated by other transparent tasks.
      !$omp task depend(inout:M(i*N_COLS))
        do j = 0, ROWS_PER_TASK-1
          do k = 0, N_COLS-1
            M((i+j)*N_COLS+k) = M((i+j)*N_COLS+k) * v(k)
          end do
        end do
      !$omp end task
    end do
  end subroutine
end program
