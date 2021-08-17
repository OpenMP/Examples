! @@name:	init_lock.1f
! @@type:	F-fixed
! @@compilable:	yes
! @@requires:	preprocessing
! @@linkable:	no
! @@expect:	success
! @@version:	omp_4.5
#if _OPENMP  < 201811
#define OMP_SYNC_HINT_CONTENDED   OMP_LOCK_HINT_CONTENDED
#define OMP_SYNC_HINT_SPECULATIVE OMP_LOCK_HINT_SPECULATIVE
#endif

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
