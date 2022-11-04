! @@name:	task_dep.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_4.0
program example
   integer :: x
   !$omp parallel
   !$omp single
      !$omp task shared(x) depend(out: x)
         x = 1
      !$omp end task
      !$omp task shared(x) depend(out: x)
         x = 2
      !$omp end task
      !$omp taskwait
      print*, "x = ", x
   !$omp end single
   !$omp end parallel
end program
