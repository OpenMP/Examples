! @@name:	fort_sp_common.3
! @@type:	F-fixed
! @@operation:	compile
! @@expect:	success
! @@version:	pre_omp_3.0
      SUBROUTINE COMMON_GOOD3()
        COMMON /C/ X,Y
!$OMP   PARALLEL PRIVATE (/C/)
          ! do work here
!$OMP   END PARALLEL
!$OMP   PARALLEL SHARED (/C/)
          ! do work here
!$OMP   END PARALLEL
      END SUBROUTINE COMMON_GOOD3
