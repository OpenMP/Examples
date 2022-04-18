! @@name:	teams.1
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:    omp_4.0
function dotprod(B,C,N) result(sum)
use omp_lib, ONLY : omp_get_num_teams, omp_get_team_num
    real    :: B(N), C(N), sum,sum0, sum1
    integer :: N, i
    sum0 = 0.0e0
    sum1 = 0.0e0
    !$omp target map(to: B, C) map(tofrom: sum0, sum1)
    !$omp teams num_teams(2)
      if (omp_get_num_teams() /= 2) stop "2 teams required"
      if (omp_get_team_num() == 0) then
         !$omp parallel do reduction(+:sum0)
         do i=1,N/2
            sum0 = sum0 + B(i) * C(i)
         end do
      else if (omp_get_team_num() == 1) then
         !$omp parallel do reduction(+:sum1)
         do i=N/2+1,N
            sum1 = sum1 + B(i) * C(i)
         end do
      end if
    !$omp end teams
    !$omp end target
    sum = sum0 + sum1
end function

! Note:  The variables sum0,sum1 are now mapped with tofrom, for correct 
! execution with 4.5 (and pre-4.5) compliant compilers. See Devices Intro.
