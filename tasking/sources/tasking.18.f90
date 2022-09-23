! @@name:	tasking.18
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:    omp_4.0
program example
   integer :: x
   x = 1
   !$omp parallel
   !$omp single
      !$omp task shared(x) depend(out: x)
         x = 2
      !$omp end task
      !$omp task shared(x) depend(in: x)
         print*, "x + 1 = ", x+1, "."
      !$omp end task
      !$omp task shared(x) depend(in: x)
         print*, "x + 2 = ", x+2, "."
      !$omp end task
   !$omp end single
   !$omp end parallel
end program
