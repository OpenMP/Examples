! @@name:	allocators.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	ct-error
! @@version:	omp_5.2
subroutine broken_auto_deallocation
   use omp_lib
   implicit none
   integer, parameter :: align_32=32
   real, allocatable  :: c(:)

   integer(omp_memspace_handle_kind ) :: my_memspace
   type(   omp_alloctrait           ) :: my_traits(1)
   integer(omp_allocator_handle_kind) :: my_alloctr

   my_memspace  =  omp_default_mem_space
   my_traits    = [omp_alloctrait(omp_atk_alignment,align_32)]
   my_alloctr   =  omp_init_allocator(my_memspace, 1, my_traits)

   !$omp allocators allocate(my_alloctr: c)
   allocate(c(100))

   !...

   call omp_destroy_allocator(my_alloctr)
   ! Auto-deallocation of c fails, 
   ! because my_alloctr is no longer available.

end subroutine
