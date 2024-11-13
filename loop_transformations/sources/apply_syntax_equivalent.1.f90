! @@name:       apply_syntax_equivalent.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_5.1
subroutine equivalent(A)
   implicit none
   double precision :: A(0:99)
   integer          :: i1,i2, i

   !$omp unroll
   do i1=0,24
   do i2=0, 3
      i = i1 * 4 + i2
      A(i) = A(i) + 1 
   enddo; enddo

end subroutine
