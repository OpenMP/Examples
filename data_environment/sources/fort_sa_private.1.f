! @@name:	fort_sa_private.1
! @@type:	F-fixed
! @@operation:	run
! @@expect:	unspecified
! @@version:	pre_omp_3.0
       SUBROUTINE SUB()
       COMMON /BLOCK/ X
       PRINT *,X             ! X is undefined
       END SUBROUTINE SUB

       PROGRAM PRIV_RESTRICT
         COMMON /BLOCK/ X
         X = 1.0
!$OMP    PARALLEL PRIVATE (X)
         X = 2.0
         CALL SUB()
!$OMP    END PARALLEL
      END PROGRAM PRIV_RESTRICT
