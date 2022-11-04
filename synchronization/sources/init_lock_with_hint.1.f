! @@name:	init_lock_with_hint.1
! @@type:	F-fixed
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.0
      FUNCTION NEW_LOCKS()
        USE OMP_LIB        ! or INCLUDE "omp_lib.h"
        INTEGER(OMP_LOCK_KIND), DIMENSION(1000) :: NEW_LOCKS

        INTEGER I

!$OMP   PARALLEL DO PRIVATE(I)
          DO I=1,1000
            CALL OMP_INIT_LOCK_WITH_HINT(NEW_LOCKS(I), 
     &             OMP_SYNC_HINT_CONTENDED + OMP_SYNC_HINT_SPECULATIVE)
          END DO
!$OMP   END PARALLEL DO

      END FUNCTION NEW_LOCKS
