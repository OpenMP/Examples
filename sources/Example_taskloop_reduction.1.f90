! @@name:	taskloop_reduction.1.f90
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:    omp_5.0

function array_sum(n, v) result(res)
    implicit none
    integer :: n, v(n), res
    integer :: i

    res = 0
    !$omp taskloop reduction(+: res)
    do i=1, n
        res = res + v(i)
    end do
    !$omp end taskoop

end function array_sum

program main
    implicit none
    integer :: n, v(10), res
    integer :: i

    integer, external :: array_sum

    n = 10
    do i=1, n
        v(i) = i
    end do

    !$omp parallel
    !$omp single
    res = array_sum(n, v)
    print *, "The result is", res
    !$omp end single
    !$omp end parallel
end program main
