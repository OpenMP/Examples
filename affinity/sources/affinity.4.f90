! @@name:	affinity.4
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_4.0
subroutine foo
!$omp parallel num_threads(16) proc_bind(close)
      call work()
!$omp end parallel
end subroutine
