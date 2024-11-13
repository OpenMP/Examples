! @@name:       apply_span_equivalent.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine step1(A)
   implicit none
   double precision :: A(0:127, 0:127)
   integer          :: i,i1,i2, j,j1,j2

   !$omp do collapse(2)
   !$omp interchange
   do i1 = 0, 7
   !$omp reverse
   do j1 = 0, 7

      do i2 = 0, 15
      do j2 = 0, 15
         i = i1 * 16 + i2
         j = j1 * 16 + j2
         A(j,i) = A(j,i) + 1
      enddo; enddo
   enddo; enddo

end subroutine

subroutine step2(A)
   implicit none
   double precision :: A(0:127, 0:127)
   integer          :: i,i1,i2, j,j1,j2

   !$omp do collapse(2)
   !$omp interchange
   do i1 = 0, 7
   do j1 = 7, 0, -1

      do i2 = 0, 15
      do j2 = 0, 15
         i = i1 * 16 + i2
         j = j1 * 16 + j2
         A(j,i) = A(j,i) + 1
      enddo; enddo
   enddo; enddo

end subroutine

subroutine step3(A)
   implicit none
   double precision :: A(0:127, 0:127)
   integer          :: i,i1,i2, j,j1,j2

   !$omp do collapse(2)
   do j1 = 7, 0, -1
   do i1 = 0, 7

      do i2 = 0, 15
      do j2 = 0, 15
         i = i1 * 16 + i2
         j = j1 * 16 + j2
         A(j,i) = A(j,i) + 1
      enddo; enddo
   enddo; enddo

end subroutine
