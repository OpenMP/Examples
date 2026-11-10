! @@name:      target_teams_workdistribute.3
! @@type:      F-free
! @@operation: compile
! @@expect:    success
! @@version:   omp_6.0
subroutine array_transform(aa, bb, cc, dd, ee, n)
    implicit none

    integer :: n
    real, dimension(n, n) :: aa, bb, cc, ee
    real, dimension(n)    :: dd
    real :: f

    !$omp target teams workdistribute map(to:bb,cc) &
    !$omp        map(from:aa,dd,f,ee)
        aa = bb + cc
        dd = sum(aa, 1)
        f  = minval(dd)
        ee = aa ** f
    !$omp end target teams workdistribute
end subroutine array_transform
