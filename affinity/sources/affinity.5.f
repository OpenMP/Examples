! @@name:	affinity.5
! @@type:	F-fixed
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_5.1
      PROGRAM EXAMPLE
!$OMP PARALLEL PROC_BIND(primary) NUM_THREADS(4)
      CALL WORK()
!$OMP END PARALLEL
      END PROGRAM EXAMPLE
