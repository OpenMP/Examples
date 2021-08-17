! @@name:       fort_allocatable_map.2f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     fail
! @@version:    omp_5.1
program main
  implicit none

  integer, allocatable :: a(:,:), b(:)
  integer              :: x(10,2)

  allocate(a(2,10))

  !$omp target                      ! Target 1
     a=x               ! reshape (or resize) NOT ALLOWED (implicit change)
     deallocate(a)     ! allocation status change of a NOT ALLOWED
     allocate(b(20))   ! allocation status change of b NOT ALLOWED
     print*, "ERROR: status change and resize/shaping NOT ALLOWED in target rgn."
  !$omp end target

end program
