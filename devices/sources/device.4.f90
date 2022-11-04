! @@name:	device.4
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
program device_mem
  use omp_lib
  use, intrinsic            :: iso_c_binding

  integer(kind=4),parameter :: N = 10
  type(c_ptr)               :: cp
  integer(c_int), pointer   :: fp(:)
  integer(c_int)            :: rc, host_dev, targ_dev
  integer(c_size_t)         :: int_bytes

  integer, pointer  :: fp_src(:), fp_dst(:)    ! Section 2 vars
  type(c_ptr)       :: cp_src,    cp_dst       ! Section 2 vars

  integer, pointer  :: h_fp(:)                 ! Section 3 vars
  type(c_ptr)       :: h_cp,    d_cp           ! Section 3 vars

  integer :: i

  host_dev  = omp_get_initial_device()
  targ_dev  = omp_get_default_device()
  int_bytes = C_SIZEOF(rc)

!------------------------------------------------Section 1 vv-----------
  cp = omp_target_alloc(N*int_bytes, targ_dev)
  
  !$omp target is_device_ptr(cp) device(targ_dev) !fp implicit map
     call c_f_pointer(cp, fp, [ N ])              !fp becomes associated
     fp(:) = 4        
     if( all(fp == 4) ) print*,"PASSED 1 of 5"
     nullify(fp)                   !fp must be returned as disassociated
  !$omp end target 

  call omp_target_free(cp, targ_dev)
  cp = c_null_ptr

!------------------------------------------------Section 2 vv-----------

   cp_src = omp_target_alloc((N+1)*int_bytes, host_dev)
   cp_dst = omp_target_alloc(  N  *int_bytes, targ_dev)

!           Initialize host array (src)
   call c_f_pointer(cp_src, fp_src, [N+1])
   fp_src = [(i,i=1,N+1)]

   !$omp target device(targ_dev) is_device_ptr(cp_dst)
     call c_f_pointer(cp_dst, fp_dst, [N])   ! fp_dst becomes associated
     fp_dst(:) = -1                          ! Initial device storage
     nullify(fp_dst)                         ! return as disassociated
   !$omp end target

!  Copy subset of host (src) array to device (dst) array
   rc = omp_target_memcpy(                                             &
            dst=cp_dst,             src=cp_src,    length=N*int_bytes, &
            dst_offset=0_c_size_t,  src_offset=int_bytes,              &
            dst_device_num=targ_dev,src_device_num=host_dev)

!  Check dst array on device

   !$omp target device(targ_dev) is_device_ptr(cp_dst)
     call c_f_pointer(cp_dst, fp_dst, [N])
     if ( all(fp_dst == [(i,i=1,N)]) ) print*,"PASSED 2 of 5"
     nullify(fp_dst)
   !$omp end target

!------------------------------------------------Section 3 vv-----------

   !allocate host memory and initialize.
   allocate(h_fp(N), source=[(i,i=1,N)])

   h_cp = c_loc(h_fp)
             ! Device is not aware of allocation on host
   if(omp_target_is_present(h_cp, targ_dev) == 0) &
      print*, "PASSED 3 of 5"

             ! Allocate device memory
   d_cp = omp_target_alloc(c_sizeof(h_fp(1))*size(h_fp), targ_dev)

             ! now associate host and device storage
   rc=omp_target_associate_ptr(h_cp,d_cp,c_sizeof(h_fp(1))*size(h_fp), &
                               0_c_size_t,targ_dev)

             ! check presence of device data, associated w. host pointer
   if(omp_target_is_present(h_cp, targ_dev) /= 0) &
      print*,"PASSED 4 of 5"

             ! copy from host to device via C pointers
   rc=omp_target_memcpy(d_cp,       h_cp,c_sizeof(h_fp(1))*size(h_fp), &
                        0_c_size_t, 0_c_size_t,                        &
                        targ_dev,   host_dev)

             ! validate the device data in the target region
             ! no data copy here since the reference count is infinity
   !$omp target device(targ_dev) map(h_fp)
     if ( all(h_fp == [(i,i=1,N)]) ) print*, "PASSED 5 of 5"
   !$omp end target

   call omp_target_free(d_cp,targ_dev)
   deallocate(h_fp)
end program
