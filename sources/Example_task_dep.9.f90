! @@name:	task_dep.6f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:    omp_5.0
program example
   integer :: a, b, c, d
   !$omp parallel
   !$omp single
      !$omp task depend(out: c)
      c = 1      ! Task T1
      !$omp end task
      !$omp task depend(out: a)
      a = 2      ! Task T2
      !$omp end task
      !$omp task depend(out: b)
      b = 3      ! Task T3
      !$omp end task
      !$omp task depend(in: a) depend(mutexinoutset: c)
      c = c + a  ! Task T4
      !$omp end task
      !$omp task depend(in: b) depend(mutexinoutset: c)
      c = c + b  ! Task T5
      !$omp end task
      !$omp task depend(in: c)
      d = c      ! Task T6
      !$omp end task
   !$omp end single
   !$omp end parallel
   print *, d
end program
