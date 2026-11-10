! @@name:	task_dep.16
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.1
program main
  implicit none
  integer :: x, y

  ! Case 1: T0 completes before T2 due to task dependence on x
  y = 0
  !$omp parallel masked
    !$omp task transparent(omp_export) depend(out:x) ! T0
      !$omp task depend(out:y) ! T1
        y = 1
      !$omp end task
    !$omp end task
    !$omp task depend(in: x, y) ! T2
        if (y /= 1) error stop
    !$omp end task   
  !$omp end parallel masked

  ! Case 2: U0 completes before U2 due to being undeferred
  y = 0
  !$omp parallel masked
    !$omp task transparent(omp_export) if(.false.) ! U0
      !$omp task depend(out:y) ! U1
        y = 1
      !$omp end task
    !$omp end task
    !$omp task depend(in: y) ! U2
        if (y /= 1) error stop
    !$omp end task   
  !$omp end parallel masked

  ! Case 3: V0 completes before V2 due to taskwait
  y = 0
  !$omp parallel masked
    !$omp task transparent(omp_export) ! V0
      !$omp task depend(out:y) ! V1
        y = 1
      !$omp end task
    !$omp end task
    !$omp taskwait
    !$omp task depend(in: y) ! V2
        if (y /= 1) error stop
    !$omp end task   
  !$omp end parallel masked

end program
