! @@name:	threadprivate.1
! @@type:	F-fixed
! @@operation:	compile
! @@expect:	success
! @@version:	pre_omp_3.0
      INTEGER FUNCTION INCREMENT_COUNTER()
        COMMON/INC_COMMON/COUNTER
!$OMP   THREADPRIVATE(/INC_COMMON/)

        COUNTER = COUNTER +1
        INCREMENT_COUNTER = COUNTER
        RETURN
      END FUNCTION INCREMENT_COUNTER
