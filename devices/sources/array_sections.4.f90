! @@name:	array_sections.4
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
subroutine foo()
integer,target  :: A(30)
integer,pointer :: p(:)
   !$omp target data map( A(1:10) )
     p=>A
     !$omp target map( p(4:10) )
        A(3) = 0
        p(9) = 0
        A(9) = 1
     !$omp end target
   !$omp end target data
end subroutine
