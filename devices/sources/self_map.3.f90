! @@name:	self_map.3
! @@type:	F-free
! @@operation:	run
! @@expect:	rt-error
! @@version:	omp_6.0

module bad_self_maps
   private :: is_accessible
contains

   function is_accessible(p)
      use omp_lib
      use, intrinsic :: iso_c_binding
      logical :: is_accessible
      integer, target :: p(:)
      integer :: n
      type(c_ptr) :: cp
      n = size(p)
      cp = c_loc(p)
      is_accessible = omp_target_is_accessible(cp, n * c_sizeof(p(1)), &
                                               omp_get_default_device()) /= 0
   end function

   function is_self_mapped(p)
      use omp_lib
      use, intrinsic :: iso_c_binding
      logical :: is_self_mapped
      integer, target :: p(:)
      integer :: n
      type(c_ptr) :: cp
      n = size(p)
      cp = c_loc(p)
      is_self_mapped = omp_get_mapped_ptr(cp, omp_get_default_device()) == cp
   end function

   ! Case 1: attempted self map when p is not accessible from device
   ! will result in a runtime error if the implementation is unable to
   ! make it accessible during the map.
   subroutine self_map_inaccessible_memory()
      integer, parameter :: n = 100
      integer, pointer :: p(:)
      logical :: accessible

      allocate(p(n))
      accessible = is_accessible(p)
      if (.not. accessible) then
         !$omp target_data map(self: p) ! potential runtime error
            ! ...
         !$omp end target_data
      end if
   end subroutine

   ! Case 2: self map storage that is already mapped without a self map
   ! will result in a runtime error
   subroutine self_map_mapped_storage()
      integer, parameter :: n = 100
      integer, pointer :: p(:)
      logical :: self_mapped

      allocate(p(n))
      !$omp target_data map(p)
         self_mapped = is_self_mapped(p)
         if (.not. self_mapped) then
            ! p is now mapped without a self map, but try to self map it anyway
            !$omp target_data map(self: p) ! runtime error
               ! ...
            !$omp end target_data
         end if
      !$omp end target_data
   end subroutine

   ! Case 3: self mapping a pointer that would get a different value due to
   ! pointer attachment will result in a runtime error.
   subroutine self_map_pointer_with_attachment()
      integer, parameter :: n = 100
      integer, target :: x(n)
      integer, pointer :: p(:)
      logical :: self_mapped

      p => x
      !$omp target_data map(x)
         self_mapped = is_self_mapped(x)
         if (.not. self_mapped) then
            ! x is now mapped without a self map, but try (in vain) to self
            ! map a pointer with pointer attachment to it.
            !$omp target_data map(self: p) map(p(:)) ! runtime error
               ! ...
            !$omp end target_data
         end if
      !$omp end target_data
   end subroutine
end module

program main
   use bad_self_maps
   call self_map_inaccessible_memory()
   call self_map_mapped_storage()
   call self_map_pointer_with_attachment()
end program
