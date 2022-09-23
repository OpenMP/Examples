! @@name:	associate.2
! @@type:	F-fixed
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_4.0
      program example
      use omp_lib
      integer  i
!$omp parallel private(i)
      i = omp_get_thread_num()
      associate(thread_id => i)
        print *, thread_id       ! print private i value
      end associate
!$omp end parallel
      end program
