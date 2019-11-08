! @@name:	task_dep.11f90
! @@type:	F-free
! @@compilable:	yes, omp_5.0
! @@linkable:	no
! @@expect:	success

subroutine set_an_element(e, val)
    implicit none
    integer :: e, val

    e = val

end subroutine 

subroutine print_all_elements(v, n)
    implicit none
    integer :: n, v(n)

    print *, v

end subroutine 

subroutine parallel_computation(n)
    implicit none
    integer :: n
    integer :: i, v(n)
  
    !$omp parallel
    !$omp single
        do i=1, n
            !$omp task depend(out: v(i))
                 all set_an_element(v(i), i)
            !$omp end task
        enddo

        !$omp task depend(iterator(it = 1:n), in: v(it))
       !!$omp task depend(in: v(1:n)) Violates Array section restriction.
            call print_all_elements(v, n)
        !$omp end task

    !$omp end single
    !$omp end parallel
end subroutine
