! @@name:       fort_allocatable_map.1f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:    omp_5.1
program main
  implicit none
  integer :: i

  integer, save, allocatable :: d(:)
  !$omp    declare target(d)

  integer, allocatable :: a(:)
  integer, allocatable :: b(:)
  integer, allocatable :: c(:)

  allocate(a(4))
  !$omp target                      ! Target 1
    a(:) = 4
  !$omp end target
  print *, a ! prints 4*4

  allocate(b(4))
  !$omp target map(b)               ! Target 2 
    b(:) = 4
  !$omp end target
  print *, b ! prints 4*4

  !$omp target data map(c)
  
    allocate(c(4), source=[1,2,3,4])
    !$omp target map(always,tofrom:c) ! Target 3 
       c(:) = 4
    !$omp end target 
    print *, c ! prints 4*4

    deallocate(c)
  
  !$omp end target data

  allocate(d(4), source=[1,2,3,4])
  !$omp target map(always,tofrom:d) ! Target 4
     d(:) = d(:) + [ ( i,i=size(d),1,-1) ]
  !$omp end target
  print *, d ! prints 4*5

  deallocate(a, b, d)

end program
