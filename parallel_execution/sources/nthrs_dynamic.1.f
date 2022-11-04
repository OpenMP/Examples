! @@name:	nthrs_dynamic.1
! @@type:	F-fixed
! @@operation:	link
! @@expect:	success
! @@version:	pre_omp_3.0
      PROGRAM EXAMPLE
        INCLUDE "omp_lib.h"      ! or USE OMP_LIB
        CALL OMP_SET_DYNAMIC(.FALSE.)
!$OMP     PARALLEL NUM_THREADS(10)
            ! do work here
!$OMP     END PARALLEL
      END PROGRAM EXAMPLE
