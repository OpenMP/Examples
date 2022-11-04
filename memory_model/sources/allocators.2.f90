! @@name:	allocators.2
! @@type:	F-free
! @@operation:	link
! @@expect:	success
! @@version:	omp_5.2
program main
   use omp_lib
   implicit none

   integer, parameter :: align_32=32
   real, allocatable  :: a(:,:)
   real               :: b(10,10)

   integer(omp_memspace_handle_kind ) :: my_memspace
   type(   omp_alloctrait           ) :: my_traits(1)
   integer(omp_allocator_handle_kind) :: my_alloctr

   my_memspace  =  omp_default_mem_space
   my_traits    = [omp_alloctrait(omp_atk_alignment,align_32)]
!                                     allocator alignment ^^
   my_alloctr   =  omp_init_allocator(my_memspace, 1, my_traits)

   !$omp allocators allocate(allocator(my_alloctr), align(64): a)
   allocate(a(5,5)) ! 64-byte aligned by clause <---------^^    

   a = b  ! reallocation occurs with 32-byte alignment
          ! uses just my_alloctr (32-byte align from allocator)

   deallocate(a)  ! Uses my_alloctr in deallocation.
   call omp_destroy_allocator(my_alloctr)

end program main
