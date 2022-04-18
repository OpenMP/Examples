! @@name:	mem_model.1
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	rt-error
! @@version:	omp_3.1
PROGRAM MEMMODEL
  INCLUDE "omp_lib.h"      ! or USE OMP_LIB
  INTEGER X, XVAL

  X = 2
!$OMP PARALLEL NUM_THREADS(2) SHARED(X)

    IF (OMP_GET_THREAD_NUM() .EQ. 0) THEN
    !$OMP ATOMIC WRITE
       X = 5
    ELSE
    !$OMP ATOMIC READ
      XVAL = X
    ! PRINT 1: XVAL can be 2 or 5
      PRINT *,"1: THREAD# ", OMP_GET_THREAD_NUM(), "X = ", XVAL
    ENDIF

 !$OMP BARRIER

    IF (OMP_GET_THREAD_NUM() .EQ. 0) THEN
    ! PRINT 2
      PRINT *,"2: THREAD# ", OMP_GET_THREAD_NUM(), "X = ", X
    ELSE
    ! PRINT 3
      PRINT *,"3: THREAD# ", OMP_GET_THREAD_NUM(), "X = ", X
    ENDIF

!$OMP END PARALLEL

END PROGRAM MEMMODEL
