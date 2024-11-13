! @@name:	selector_scoring.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
module subs
   ! The unified_address and/or unified_shared_memory requirements may be
   ! implicitly enforced by the implementation via compiler flags.

   integer, parameter :: version = 2
contains

   subroutine kernel(a, n)
      !$omp declare variant(kernel_target_ua) &
      !$omp     match(implementation={requires(unified_address)})

      !$omp declare variant(kernel_target_usm) &
      !$omp     match(implementation={requires(unified_shared_memory)})

      !$omp declare variant(kernel_target_usm_v2) &
      !$omp     match(implementation={requires(unified_shared_memory)}, &
      !$omp           user={condition(score(1): version==2)})

      integer, target :: a(n)
      integer, value :: n
      integer :: i
      !$omp parallel do
      do i = 1, n
         a(i) = i*i
      end do
   end subroutine

   subroutine kernel_target_ua(a, n)
      use iso_c_binding
      integer, target :: a(n)
      integer, value :: n
      type(c_ptr) :: c_ap
      integer, pointer :: ap(:)
      integer :: i
      c_ap = c_loc(a)
      ap => null()
      !$omp target data map(a(:n)) use_device_ptr(c_ap)
      !$omp target
         call c_f_pointer(c_ap, ap, [n])
         !$omp parallel do
         do i = 1, n
            ap(i) = 2*i*i
         end do
         ap => null() ! reset pointer association status
      !$omp end target
      !$omp end target data
   end subroutine

   subroutine kernel_target_usm(a, n)
      integer, target :: a(n)
      integer, value :: n
      integer :: i
      !$omp target parallel do
      do i = 1, n
         a(i) = 3*i*i
      end do
   end subroutine

   subroutine kernel_target_usm_v2(a, n)
      integer, target :: a(n)
      integer, value :: n
      integer :: i
      !$omp target teams loop
      do i = 1, n
         a(i) = 4*i*i
      end do
   end subroutine

end module subs


program main
   use subs
   integer, target :: a(1000)

   call kernel(a, 1000)

   do i = 1, 1000
      if (a(i) /= 4*i*i ) then
         print *, "Failed"
         error stop
      end if
   end do

   print *, "Passed"

end program
