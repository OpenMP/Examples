! @@name:	array_sections.1f
! @@type:	F-free
! @@compilable:	no
! @@linkable:	no
! @@expect:	failure
subroutine foo()
integer :: A(30)
   A = 1
   !$omp target data map( A(1:4) )
     ! Cannot map distinct parts of the same array
     !$omp target map( A(8:27) )
        A(3) = 0
     !$omp end target map
   !$omp end target data
end subroutine
