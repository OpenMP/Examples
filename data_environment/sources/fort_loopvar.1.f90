! @@name:	fort_loopvar.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	pre_omp_3.0
SUBROUTINE PLOOP_1(A,N)
INCLUDE "omp_lib.h"      ! or USE OMP_LIB

REAL A(*)
INTEGER I, MYOFFSET, N

!$OMP PARALLEL PRIVATE(MYOFFSET)
       MYOFFSET = OMP_GET_THREAD_NUM()*N
       DO I = 1, N
         A(MYOFFSET+I) = FLOAT(I)
       ENDDO
!$OMP END PARALLEL

END SUBROUTINE PLOOP_1
