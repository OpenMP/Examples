! @@name:	init_lock.1f
! @@type:	F-fixed
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
      FUNCTION NEW_LOCKS()
        USE OMP_LIB        ! or INCLUDE "omp_lib.h"
        INTEGER(OMP_LOCK_KIND), DIMENSION(1000) :: NEW_LOCKS

        INTEGER I

!$OMP   PARALLEL DO PRIVATE(I)
          DO I=1,1000
            CALL OMP_INIT_LOCK_WITH_HINT(NEW_LOCKS(I), 
     &             OMP_LOCK_HINT_CONTENDED + OMP_LOCK_HINT_SPECULATIVE)
          END DO
!$OMP   END PARALLEL DO

      END FUNCTION NEW_LOCKS
