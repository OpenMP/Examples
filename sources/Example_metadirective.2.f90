! @@name: metadirective.2f90
! @@type: F-free
! @@compilable: yes, omp_5.0
! @@linkable: no
! @@expect: success

program main                    !!Driver
  use omp_lib
  implicit none
  integer, parameter :: N=1000
  external           :: work_on_chunk
  integer            :: i,idev

  do idev=0,omp_get_num_devices()-1

    !$omp target device(i)
    !$omp metadirective \
    !$omp&  when( implementation={vendor(nvidia)}, device={arch("kepler")}: &
    !$omp         teams num_teams(512) thread_limit(32) )                   &
    !$omp&  when( implementation={vendor(amd)},    device={arch("fiji"  )}: &
    !$omp&        teams num_teams(512) thread_limit(64) )                   &
    !$omp&  default(                                                        &
    !$omp&        teams)
    !$omp distribute parallel for
    do i=1,N  
       call work_on_chunk(idev,i)
    end do

  end do

end program

