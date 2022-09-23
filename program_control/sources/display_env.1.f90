! @@name:	display_env.1
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_5.1
!implementers: customize debug routines for app debugging
function  debug()
  logical :: debug
  debug = .true.  
end function

function debug_omp_verbose()
  logical :: debug_omp_verbose
  debug_omp_verbose = .false.
end function

program display_omp_environment
  use omp_lib
  logical :: debug, debug_omp_verbose
 
  if( debug() ) call omp_display_env( debug_omp_verbose() )
  !! ...
end program

