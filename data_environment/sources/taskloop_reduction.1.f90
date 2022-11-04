! @@name:	taskloop_reduction.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
function array_sum(n, v) result(res)
    implicit none
    integer :: n, v(n), res
    integer :: i

    res = 0
    !$omp taskloop reduction(+: res)
    do i=1, n
        res = res + v(i)
    end do
    !$omp end taskloop

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
