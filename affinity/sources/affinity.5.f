! @@name:	affinity.5f
! @@type:	F-fixed
! @@compilable:	yes
! @@requires:	preprocessing
! @@linkable:	no
! @@expect:	success
! @@version:	omp_5.1
#if _OPENMP  < 202011
#define primary master
#endif

      PROGRAM EXAMPLE
!$OMP PARALLEL PROC_BIND(primary) NUM_THREADS(4)
      CALL WORK()
!$OMP END PARALLEL
      END PROGRAM EXAMPLE
