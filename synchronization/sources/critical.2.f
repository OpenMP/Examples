! @@name:	critical.1f
! @@type:	F-fixed
! @@compilable:	yes
! @@requires:	preprocessing
! @@linkable:	no
! @@expect:	success
! @@version:	omp_4.5
#if _OPENMP  < 201811
#define OMP_SYNC_HINT_CONTENDED   OMP_LOCK_HINT_CONTENDED
#endif

      SUBROUTINE CRITICAL_EXAMPLE(X, Y)
        USE OMP_LIB        ! or INCLUDE "omp_lib.h"
        
        REAL X(*), Y(*)
        INTEGER IX_NEXT, IY_NEXT

!$OMP PARALLEL SHARED(X, Y) PRIVATE(IX_NEXT, IY_NEXT)

!$OMP CRITICAL(XAXIS) HINT(OMP_SYNC_HINT_CONTENDED)
        CALL DEQUEUE(IX_NEXT, X)
!$OMP END CRITICAL(XAXIS)
        CALL WORK(IX_NEXT, X)

!$OMP CRITICAL(YAXIS) HINT(OMP_SYNC_HINT_CONTENDED)
        CALL DEQUEUE(IY_NEXT,Y)
!$OMP END CRITICAL(YAXIS)
        CALL WORK(IY_NEXT, Y)

!$OMP END PARALLEL

      END SUBROUTINE CRITICAL_EXAMPLE
