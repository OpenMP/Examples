! @@name:	task_dep.4b
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.1
subroutine task_dep(N)
  implicit none
  integer :: N

  integer :: i, v, R
  integer, external :: f

  !$omp parallel private(i,v) shared(R)
  !$omp single
    !! CASE 1: tasks with inout dependence type.
    !!         tasks are serialized here.
    R = 0
    do i = 1, N
      !$omp task depend(inout: R)    !! T1
        v = f(i)
        R = R + v
      !$omp end task
    end do

    !$omp task depend(in: R)         !! T2
      print *, "result is ", R
    !$omp end task
    !$omp taskwait      !! to avoid race with CASE 2

    !! CASE 2: tasks with inoutset dependence type.
    !!         tasks are executed concurrently.
    R = 0
    do i = 1, N
      !$omp task depend(inoutset: R) !! T3
        v = f(i)
        !$omp atomic
        R = R + v
      !$omp end task
    end do

    !$omp task depend(in: R)         !! T4
      print *, "result is ", R
    !$omp end task

  !$omp end single
  !$omp end parallel
end subroutine
