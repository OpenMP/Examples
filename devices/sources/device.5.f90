! @@name:	device.5
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.2
subroutine async_memcpy
  use omp_lib
  use, intrinsic     :: iso_c_binding

  implicit none

  integer, parameter :: N = 128
  real, target       :: h_buf(N)
  type(c_ptr)        :: c_dbuf, c_hbuf
  integer(c_int)     :: d_dev, h_dev
  integer(c_size_t)  :: dsize

  integer :: i

  h_dev = omp_get_initial_device()
  d_dev = omp_get_default_device()
  dsize = N * c_sizeof(h_buf(1))

  ! allocate device memory
  c_dbuf = omp_target_alloc(dsize, d_dev)
  if (.not.c_associated(c_dbuf)) stop
  c_hbuf = c_loc(h_buf)

  ! set up host data
  h_buf = [(i*0.1, i = 1, N)]

  ! copy data from host to device asynchronously
  if (omp_target_memcpy_async(c_dbuf, c_hbuf, dsize, 0, 0, &
                              d_dev, h_dev, 0) /= 0) then
    stop
  endif

  ! do some work here at the same time
  call do_work

  ! wait for task completion
  !$omp taskwait

  call omp_target_free(c_dbuf, d_dev)

end subroutine
