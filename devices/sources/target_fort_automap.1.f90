! @@name:       target_fort_automap.1
! @@type:       F-free
! @@operation:  run
! @@expect:     success
! @@version:    omp_6.0
module my_data
  integer, allocatable :: aa(:)
  !$omp declare_target enter(automap:aa)
end module

program main
  use omp_lib
  use my_data
  implicit none
  integer, parameter :: N=10

  allocate(aa(N))         !! Device allocation occurs here

  aa = 1
  !$omp target_update to(aa)
  !$omp target            !! aa is implicitly mapped
    aa = aa + 1
  !$omp end target
  !$omp target_update from(aa)

  if ( all(aa == 2) ) then
    print *, "passed"
  else
    print *, "failed"
  end if

  deallocate(aa)          !! Device deallocation occurs here

end program
