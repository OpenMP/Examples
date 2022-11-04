! @@name:	array_sections.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	ct-error
! @@version:	omp_4.0
subroutine foo()
integer :: A(30)
   A = 1
   !$omp target data map( A(1:4) )
     ! Cannot map distinct parts of the same array
     !$omp target map( A(8:27) )
        A(3) = 0
     !$omp end target
   !$omp end target data
end subroutine
