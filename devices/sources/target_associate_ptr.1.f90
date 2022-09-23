! @@name:       target_associate_ptr.1
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:	omp_5.1
program target_associate
  use omp_lib
  use, intrinsic :: iso_c_binding
  implicit none

  integer, parameter :: CS = 50
  integer, parameter :: N  = CS*2
  integer, target :: arr(N)
  type(c_ptr) :: h_ptr, dev_ptr
  integer(c_size_t) :: csize, dev_off
  integer(c_int) :: dev
  integer :: i, ioff, s

  do i = 1, N
    arr(i) = i
  end do

  dev = omp_get_default_device()
  csize = c_sizeof(arr(1)) * CS

  ! Allocate device memory
  dev_ptr = omp_target_alloc(csize, dev)
  dev_off = 0

  ! Loop over chunks
  do ioff = 1, N, CS

    ! Associate device memory with one chunk of host memory
    h_ptr = c_loc(arr(ioff))
    s = omp_target_associate_ptr(h_ptr, dev_ptr, csize, dev_off, dev)

    print *, "before: arr(", ioff, ")=", arr(ioff)

    ! Update the device data
    !$omp target update to(arr(ioff:ioff+CS-1)) device(dev)

    ! Explicit mapping of arr to make sure that we use the allocated 
    ! and associated memory.  No host-device data update here.
    !$omp target map(tofrom: arr(ioff:ioff+CS-1)) device(dev)
      do i = 0, CS-1
        arr(i+ioff) = arr(i+ioff) + 1
      end do
    !$omp end target

    ! Update the host data
    !$omp target update from(arr(ioff:ioff+CS-1)) device(dev)

    print *, "after: arr(", ioff, ")=", arr(ioff)

    ! Disassociate device pointer from the current chunk of host memory
    ! before next use
    s = omp_target_disassociate_ptr(h_ptr, dev)
  end do

  ! Free device memory
  call omp_target_free(dev_ptr, dev)

end
! Outputs:
!  before: arr( 1 )= 1
!  after: arr( 1 )= 2
!  before: arr( 51 )= 51
!  after: arr( 51 )= 52
