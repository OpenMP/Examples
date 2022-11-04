! @@name:	task_dep.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_4.0
program example
   integer :: x
   x = 1
   !$omp parallel
   !$omp single
      !$omp task shared(x) depend(out: x)
         x = 2
      !$omp end task
      !$omp task shared(x) depend(in: x)
         print*, "x = ", x
      !$omp end task
   !$omp end single
   !$omp end parallel
end program
