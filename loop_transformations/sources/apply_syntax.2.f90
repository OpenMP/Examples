! @@name:       apply_syntax.2
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine apply_assoc(A)
   implicit none
   double precision :: A(0:2, 0:99, 0:99)
   integer          ::    k,   j,    i   

   !$omp  tile sizes(4,4) &
   !$omp&      apply(     grid: interchange, nothing) &
   !$omp&      apply(intratile: nothing, interchange)
   do i = 0, 99
   do j = 0, 99

      do k = 0, 2 !! k loop not associated with tile, but w. interchange
         A(k,j,i) = A(k,j,i) + 1
      enddo
   enddo; enddo
end subroutine
