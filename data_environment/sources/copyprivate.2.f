! @@name:	copyprivate.2f
! @@type:	F-fixed
! @@compilable:	yes
! @@requires:	preprocessing
! @@linkable:	no
! @@expect:	success
! @@version:    omp_5.1
#if _OPENMP  < 202011
#define MASKED MASTER
#endif

        REAL FUNCTION READ_NEXT()
        REAL, POINTER :: TMP

!$OMP   SINGLE
          ALLOCATE (TMP)
!$OMP   END SINGLE COPYPRIVATE (TMP)  ! copies the pointer only

!$OMP   MASKED
          READ (11) TMP
!$OMP   END MASKED

!$OMP   BARRIER
          READ_NEXT = TMP
!$OMP   BARRIER

!$OMP   SINGLE
          DEALLOCATE (TMP)
!$OMP   END SINGLE NOWAIT
        END FUNCTION READ_NEXT
