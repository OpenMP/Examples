! @@name:	target_ptr_map.5
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.2
program main
   use omp_lib

   use, intrinsic :: iso_c_binding, only : c_loc, c_size_t, c_sizeof, c_int
   implicit none
   external :: do_work

   type T
     integer,pointer :: ptr(:)
     integer         :: buf_size
   end type

   !$omp declare mapper(deep_copy: T :: s) map(s, s%ptr(:s%buf_size))

   integer,parameter :: n = 1000
   integer(c_int)    :: dev, accessible
   integer(c_size_t) :: buf_size

   type(T) s

   allocate(s%ptr(n))

   buf_size = c_sizeof(s%ptr(1))*n
   dev = omp_get_default_device()

   accessible = omp_target_is_accessible(c_loc(s%ptr(1)), buf_size, dev)

   !$omp  begin metadirective                              &
   !$omp&       when(user={condition(accessible)}: target) &
   !$omp&       otherwise( target map(mapper(deep_copy),tofrom:s) )

      call do_work(s, n)

   !$omp  end   metadirective

   deallocate(s%ptr)

end program
