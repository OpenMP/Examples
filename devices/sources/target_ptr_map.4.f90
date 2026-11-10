! @@name:	target_ptr_map.4
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.2
program main
   use omp_lib
   use, intrinsic :: iso_c_binding, only : c_loc, c_size_t, c_sizeof, c_int
   implicit none

   external           :: do_work
   integer, parameter :: n = 1000

   integer(c_int)     :: dev, accessible
   integer(c_size_t)  :: buf_size
   integer, pointer   :: ptr(:)

   ! allocated memory may or may not be accessible on a non-host device
   allocate(ptr(n))

   buf_size = c_sizeof(ptr(1)) * n
   dev = omp_get_default_device()

   accessible = omp_target_is_accessible(c_loc(ptr(1)), buf_size, dev)

   !$omp begin metadirective                                   &
   !$omp&      when(user={condition(accessible /= 0)}: target) &
   !$omp&      otherwise( target map(ptr(1:n)) )
      call do_work(ptr, n)
   !$omp  end  metadirective

   deallocate(ptr)

end program
