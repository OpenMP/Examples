! @@name:	teams.7
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
PROGRAM main
  USE omp_lib
  INTEGER :: x
  !$OMP DECLARE TARGET LOCAL(x)

   x = 128

  !$OMP TARGET
   x = 256
  !$OMP END TARGET 

  !$OMP TARGET
  !$OMP TEAMS NUM_TEAMS(x)        ! Undefined behavior due to value of 'x'
  IF (omp_get_team_num() == 0) THEN
    PRINT *, omp_get_num_teams()  
  END IF
  !$OMP END TEAMS
  !$OMP END TARGET

END PROGRAM main
