! @@name:	task_dep.12f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_4.0
program example
   integer :: x
   x = 0
   !$omp parallel
   !$omp single
     !... first explicit task
      !$omp task shared(x) depend(out: x)
         x = 1
      !$omp end task

     !... second explicit task
      !$omp task shared(x) depend(inout: x) if(.false.)
         x = 2
      !$omp end task

     !... statement executed by parent implicit task
     ! prints: x = 2
      print*, "x = ", x
   !$omp end single
   !$omp end parallel
end program
