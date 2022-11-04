! @@name:	metadirective.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.2
program main                    !!Driver
  use omp_lib
  implicit none
  integer, parameter :: N=1000
  external           :: work_on_chunk
  integer            :: i,idev

  do idev=0,omp_get_num_devices()-1

    !$omp target device(idev)
    !$omp begin metadirective &
    !$omp&  when( implementation={vendor(nvidia)},        &
    !$omp&           device={arch("kepler")}:             &
    !$omp&        teams num_teams(512) thread_limit(32) ) &
    !$omp&  when( implementation={vendor(amd)},           &
    !$omp&           device={arch("fiji"  )}:             &
    !$omp&        teams num_teams(512) thread_limit(64) ) &
    !$omp&  otherwise( teams )
    !$omp distribute parallel do
    do i=1,N  
       call work_on_chunk(idev,i)
    end do
    !$omp end metadirective
    !$omp end target

  end do

end program
