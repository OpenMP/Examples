! @@name:       apply_span.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine span_apply( A )
   implicit none
   double precision :: A(0:127,0:127)
   integer          ::   i   , j

    !$omp for collapse(2)
    !$omp tile sizes(16,16) apply(grid: interchange,reverse)
    do i = 0, 127
    do j = 0, 127
      A(j,i) = A(j,i) + 1
    enddo; enddo

end subroutine
