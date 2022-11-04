! @@name:	fort_sa_private.3
! @@type:	F-fixed
! @@operation:	run
! @@expect:	unspecified
! @@version:	pre_omp_3.0
      PROGRAM PRIV_RESTRICT3
        EQUIVALENCE (X,Y)
        X = 1.0

!$OMP   PARALLEL PRIVATE(X)
          PRINT *,Y                  ! Y is undefined
          Y = 10
          PRINT *,X                  ! X is undefined
!$OMP   END PARALLEL
      END PROGRAM PRIV_RESTRICT3
