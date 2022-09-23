! @@name:       error.1
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:    omp_5.2
program main
use omp_lib

!$omp  metadirective  &
!$omp&     when( implementation={vendor(gnu)}: nothing    ) &
!$omp&     otherwise( error at(compilation) severity(fatal) &
!$omp&                message( "GNU compiler required." ) )


if( omp_get_num_procs() < 3 ) then
   !$omp  error at(runtime) severity(fatal) &
   !$omp&       message("3 or more procs required.")
endif

  !$omp parallel master

!! Give notice about master deprecation at compile time and run time.
  !$omp  error at(compilation) severity(warning) &
  !$omp&       message("Notice: master is deprecated.")
  !$omp  error at(runtime) severity(warning) &
  !$omp&       message("Notice: masked to be used in next release.")

  print*," Hello from thread number 0."

  !$omp end parallel master

end program
