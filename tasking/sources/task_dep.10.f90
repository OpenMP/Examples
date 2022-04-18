! @@name:	task_dep.10
! @@type:	F-free
! @@compilable:	no
! @@linkable:	no
! @@expect:	failure
! @@version:    omp_5.0
subroutine foo
   integer :: a,b,c
   c = 0
   !$omp parallel
   !$omp single
      !$omp task depend(out: a)
         a = longTaskA()
      !$omp end task
      !$omp task depend(out: b)
         b = shortTaskB()
      !$omp end task
      !$omp task depend(in: a) depend(mutexinoutset: c)
         c = shortTaskAC(a,c)
      !$omp end task
      !$omp task depend(in: b) depend(mutexinoutset: c)
         c = longTaskBC(b,c)
      !$omp end task
   !$omp end single
   !$omp end parallel
end subroutine foo
