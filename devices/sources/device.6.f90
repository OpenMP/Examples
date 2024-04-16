! @@name:	device.6
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.2
subroutine async_work(N, d_buf, h_buf)
   use omp_lib
   use, intrinsic    :: iso_c_binding

   implicit none
   integer           :: N
   real, pointer     :: d_buf(:), h_buf(:)

   type(c_ptr)       :: c_dp, c_hp
   integer(c_int)    :: d_dev, h_dev
   integer(c_size_t) :: dsize
   integer(omp_depend_kind) :: obj(1)

   external :: do_work
   external :: do_more_work
   !$omp declare target enter(do_more_work)
   integer :: i

   h_dev = omp_get_initial_device()
   d_dev = omp_get_default_device()
   dsize = N * c_sizeof(d_buf(1))

   c_dp = c_loc(d_buf)
   c_hp = c_loc(h_buf)

   ! initialize a depend object 'obj'
   !$omp depobj(obj) depend(out: d_buf(1:N))

   ! start the async memcpy of h_buf to d_buf on device
   if (omp_target_memcpy_async(c_dp, c_hp, dsize, 0, 0, &
                               d_dev, h_dev, 1, obj) /= 0) then
      stop
   endif

   ! do some useful work at the same time on host
   call do_work(N, h_buf)

   ! wait until memcpy finishes before using d_buf in the target region
   !$omp target has_device_addr(d_buf) depend(depobj: obj)
   call do_more_work(N, d_buf)
   !$omp end target

end subroutine
