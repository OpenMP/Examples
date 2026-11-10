! @@name:	cond_comp.1
! @@type:	F-fixed
! @@operation:	run
! @@expect:	success
! @@version:	pre_omp_3.0
C23456789
      PROGRAM EXAMPLE
!$    USE OMP_LIB
      INTEGER IAM, INDEX

      IAM = 0
      INDEX = 100

!$ 10 IAM = OMP_GET_THREAD_NUM() +
!$   &      INDEX

!$    PRINT *, "Compiled by an OpenMP-compliant implementation."
      PRINT *, "iam = ", IAM

      END PROGRAM EXAMPLE
