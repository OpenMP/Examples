! @@name:	affinity.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
subroutine foo
!$omp parallel num_threads(16) proc_bind(spread)
      call work()
!$omp end parallel
end subroutine
