! @@name:      target_teams_workdistribute.4
! @@type:      F-free
! @@operation: compile
! @@expect:    rt-error
! @@version:   omp_6.0
subroutine statements_split(aa, bb, cc, n)
   implicit none

   integer :: n
   real, dimension(n, n) :: aa, bb, cc

   ! Case 1
   !$omp target teams map(to: bb,cc) map(tofrom: aa)
   !$omp workdistribute
      aa = bb + 1.0     ! S1
   !$omp end workdistribute  ! no cross-team synchronization
   !$omp workdistribute
      aa = aa + cc      ! S2, dependence on aa from S1, race
   !$omp end workdistribute
   !$omp end target teams

   ! Case 2
   !$omp target teams map(to: bb,cc) map(tofrom: aa)
   !$omp workdistribute
      aa = bb + 1.0
      aa = aa + cc      ! Fortran semantic, no race
   !$omp end workdistribute
   !$omp end target teams
end subroutine
