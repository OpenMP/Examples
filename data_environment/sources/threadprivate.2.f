! @@name:	threadprivate.2
! @@type:	F-fixed
! @@operation:	compile
! @@expect:	ct-error
! @@version:	pre_omp_3.0
      MODULE INC_MODULE
        COMMON /T/ A
      END MODULE INC_MODULE

      SUBROUTINE INC_MODULE_WRONG()
        USE INC_MODULE
!$OMP   THREADPRIVATE(/T/)
      !non-conforming because /T/ not declared in INC_MODULE_WRONG
      END SUBROUTINE INC_MODULE_WRONG
