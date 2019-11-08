! @@name: task_depend.8.f90
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success


subroutine foo()
implicit nonE
integer :: x, y
    
    x = 0
    y = 2

    !$omp task depend(inout: x) shared(x)
        x = x + 1                         !! 1st child task
    !$omp end task

    !$omp task depend(in: x) depend(inout: y) shared(x, y)
        y = y - x                         !! 2nd child task
    !$omp end task

    !$omp taskwait depend(in: x,y)

    print*, "x=", x
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
