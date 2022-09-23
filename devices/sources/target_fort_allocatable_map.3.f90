! @@name:       target_fort_allocatable_map.3
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     fail
! @@version:    omp_5.1
module corfu
contains
  subroutine foo(ain,bout)
    implicit none
    integer, allocatable, intent( in) :: ain(:)
    integer, allocatable, intent(out) :: bout(:) !"out" causes de/realloc
    !$omp declare target
    bout = ain
  end subroutine
end module

program  main
  use corfu
  implicit none

  integer, allocatable :: a(:)
  integer, allocatable :: b(:)
  allocate(a(10),b(10))
  a(:)=10
  b(:)=10

  !$omp target

  call foo(a,b) !ERROR: b deallocation/reallocation not allowed
                !  in target region

  !$omp end target

end program
