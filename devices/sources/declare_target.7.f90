! @@name: declare_target.7
! @@type: F-free
! @@operation: link 
! @@expect: success
! @@version: omp_5.2
module subs

contains
   subroutine foo()
   !$omp declare target enter(foo) device_type(nohost)
   !$omp declare variant(foo_onhost) match(device={kind(host)})
     ! device specific computation
   end subroutine

   subroutine foo_onhost()
     print *,' On host.'
   end subroutine

end module

program main

   use subs
   !$omp target
   call foo    ! calls foo() on device
               ! or foo_onhost() in case of host fallback
   !$omp end target

end program
