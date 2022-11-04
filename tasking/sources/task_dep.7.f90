! @@name:	task_dep.7
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
subroutine foo()
implicit none
integer :: x, y
    
    x = 0
    y = 2

    !$omp task depend(inout: x) shared(x)
        x = x + 1                         !! 1st child task
    !$omp end task

    !$omp task depend(in: x) depend(inout: y) shared(x, y)
        y = y - x                         !! 2nd child task
    !$omp end task

    !$omp taskwait depend(in: x)          !! 1st taskwait

    print*, "x=", x

    !! Second task may not be finished.
    !! Accessing y here would create a race condition.

    !$omp taskwait                        !! 2nd taskwait

    print*, "y=", y

end subroutine foo

program p
implicit none
    !$omp parallel
    !$omp single
       call foo()
    !$omp end single
    !$omp end parallel
end program p
