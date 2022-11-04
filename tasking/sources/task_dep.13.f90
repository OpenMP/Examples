! @@name:	task_dep.13
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.1
program main
  integer :: a=1, d=1

  !$omp parallel masked  num_threads(5)

    !$omp task                                 !! Task 1
       write(*,'("T1")')
    !$omp end task

    !$omp task depend(out: a)                  !! Task 2
       a=a+1
       write(*,'("T2 a=",i1)') a
    !$omp end task

    !$omp task depend(out: d)                  !! Task 3
       d=d+1
       write(*,'("T3 d=",i1)') d
    !$omp end task

 
    !$omp task depend(inout: omp_all_memory)   !! Task 4
       a=a+1; d=d+1
       write(*,'("T4 a=",i1," d=",i1)') a, d
    !$omp end task

    !$omp task                                 !! Task 5
       write(*,'("T5")') 
    !$omp end task
 
    !$omp task depend(in: a,d)                 !! Task 6
       a=a+1; d=d+1
       write(*,'("T6 a=",i1," d=",i1)') a, d
    !$omp end task

  !$omp end parallel masked

end program

! OUTPUT: ordered  {T2,T3 any order}, {T4}, {T6}
! T2 a=2
! T3 d=2
! T4 a=3 d=3
! T6 a=4 d=4
! OUTPUT: unordered (can appear interspersed in ordered output)
! T1
! T5
