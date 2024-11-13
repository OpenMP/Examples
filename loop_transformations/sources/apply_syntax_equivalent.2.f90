! @@name:       apply_syntax_equivalent.2
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine equivalent(A)
   implicit none
   double precision :: A(0:2,   0:99,   0:99)
   integer          ::   k,    j1,j2,  i1,i2

   !$omp interchange       !! grid modifier
   do i1 = 0, 24
   !$omp nothing           !! grid modifier
   do j1 = 0, 24

      !$omp nothing        !! intratile modifier
      do i2 = 0, 3
      !$omp interchange    !! intratile modifier
      do j2 = 0, 3

         do k = 0, 2
            i = i1 * 4 + i2
            j = j1 * 4 + j2
            A(k,j,i) = A(k,j,i) + 1
         enddo

      enddo; enddo
   enddo; enddo
end subroutine
