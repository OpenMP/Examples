! @@name:       partial_tile.2
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.1

subroutine func7(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i,j

   !$omp parallel do schedule(static)
   !$omp tile sizes(4,16)
   do i=1,100
   do j = 1, 100
      A(j,i)  = A(j,i) + 1
   end do; end do

end subroutine

subroutine func8(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i1,i2,j1,j2

   do i1 =  1, 100,  4
   do j1 =  1, 100, 16
   do i2 = i1,  i1 + 3
   do j2 = j1, min(j1+15,100)
      A(j2,i2) = A(j2,i2) + 1
   end do; end do; end do; end do

end subroutine

subroutine func9(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i1,i2,j1,j2,j

   !$omp parallel

      !$omp do schedule(static)
      do i1 =  1, 100,  4
      do j1 =  1,  96, 16
      do i2 = i1,  i1 + 3
      do j2 = j1,  j1 +15
         A(j2,i2) = A(j2,i2) + 1
      end do; end do; end do; end do
      !$omp end do nowait
   
      !$omp do schedule(static)
      do i1 =  1, 100, 4
      do i2 = i1,  i1 +3
      do  j = 97, 100
         A(j,i2) = A(j,i2) + 1
      end do; end do; end do;

   !$omp end parallel

end subroutine
