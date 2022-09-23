! @@name:       target_fort_allocatable_map.2
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     unspecified
! @@version:    omp_5.1
program main
  implicit none

  integer, allocatable :: a(:,:), b(:), c(:)
  integer              :: x(10,2)

  allocate(a(2,10))

  !$omp target
     a = x             ! Reshape (or resize) NOT ALLOWED (implicit change)

     deallocate(a)     ! Allocation status change of "a" NOT ALLOWED.

     allocate(b(20))   ! Allocation of  b *

     c = 10            ! Intrinsic assignment allocates c *

     ! * Since an explicit deallocation for b and c does not occur before 
     ! the end of the target region, the PROGRAM BEHAVIOR IS UNSPECIFIED.
  !$omp end target

end program