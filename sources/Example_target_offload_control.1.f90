! @@name: target_offload_control.1f90
! @@type: F-free
! @@compilable: yes, omp_5.0
! @@linkable: yes
! @@expect: success


module offload_policy
  implicit none
  integer, parameter :: LEN_POLICY=10
contains
  character(LEN_POLICY) function get_offload_policy()
     character(64) :: env
     integer       :: length, i
     env=repeat(' ',len(env))
                                    !policy is blank if not found *
     call get_environment_variable("OMP_TARGET_OFFLOAD",env,length)

     do i = 1,len(env)              !Makes a-z upper case
        if(iachar(env(i:i))>96) env(i:i)=achar(iachar(env(i:i))-32)
     end do

     get_offload_policy = trim(adjustl(env)) !remove peripheral spaces

     if(length==0) get_offload_policy="NOTSET"

     return

  end function

end module

program policy_test

  use omp_lib
  use offload_policy

  integer                :: i, device_num
  logical                :: on_init_dev
  character(LEN_POLICY)  :: policy

  policy = get_offload_policy() !!Get OMP_TARGET_OFFLOAD value

  if (OPENMP_VERSION < 201811) then
     print*,"Warning: OMP_TARGET_OFFLOAD NOT supported by VER.",OPENMP_VERSION
     print*,"         If OMP_TARGET_OFFLOAD is set, it will be ignored."
  endif

     !Set target device number to an unavailable device to test offload policy.
  device_num = omp_get_num_devices() + 1

                      !!Report OMP_TARGET_OFFOAD value
  select CASE (policy)
     case("MANDATORY")
                     print*,"Policy:  MANDATORY-Terminate if dev. not avail."
     case("DISABLED")
                     print*,"Policy:  DISABLED-(if supported) Only on Host."
     case("DEFAULT")
                     print*,"Policy:  DEFAULT On host if device not avail."
     case("NOTSET")
                     print*,"         OMP_TARGET_OFFLOAD is not set."
     case DEFAULT
                     print*,"         OMP_TARGET_OFFLOAD has unknown value."
                     print*,"         UPPER CASE VALUE=",policy
  end select


  on_init_dev = .FALSE.
                                      !! device# out of range--not supported
  !$omp target  device(device_num) map(tofrom: on_init_dev)
     on_init_dev=omp_is_initial_device()
  !$omp end target

  if (policy=="MANDATORY" .and. OPENMP_VERSION>=201811) then
     print*,"OMP ERROR: OpenMP 5.0 implementation ignored MANDATORY policy."
     print*,"           Termination should have occurred at target directive."
  endif

  print*, "Target executed on init dev (T|F): ", on_init_dev

end program policy_test
