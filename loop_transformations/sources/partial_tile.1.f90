! @@name:	partial_tile.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.1
subroutine func1(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i,j

   !$omp tile sizes(4,16)
   do i = 1, 100
   do j = 1, 100
      A(j,i)  = A(j,i) + 1
   end do; end do

end subroutine


subroutine func2(A)
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


subroutine func3(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i1,i2,j1,j2, j

   !! complete tiles
   do i1 =  1, 100,  4
   do j1 =  1,  96, 16
   do i2 = i1,  i1 + 3
   do j2 = j1,  j1 +15
      A(j2,i2) = A(j2,i2) + 1
   end do; end do; end do; end do

   !! partial tiles / remainder
   do i1 =  1, 100, 4
   do i2 = i1,  i1 +3
   do  j = 97, 100
      A(j,i2) = A(j,i2) + 1
   end do; end do; end do

end subroutine


subroutine func4(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i1,i2,j1,j2, j

   do i1 =  1, 100,  4

      !! complete tiles
      do j1 =  1,  96, 16
      do i2 = i1,  i1 + 3
      do j2 = j1,  j1 +15
         A(j2,i2) = A(j2,i2) + 1
      end do; end do; end do

      !! partial tiles     
      do i2 = i1,  i1 +3
      do  j = 97, 100
         A(j,i2) = A(j,i2) + 1
      end do; end do

   end do

end subroutine


subroutine func5(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i1,i2,j1,j2

   do i1 =  1, 100,  4
   do j1 =  1, 100, 16
   do i2 = i1,  i1 + 3
   do j2 = j1,  j1 +15
      if (j2 < 101) A(j2,i2) = A(j2,i2) + 1
   end do; end do; end do; end do

end subroutine


subroutine func6(A)
   implicit none
   double precision   :: A(100,100)
   integer            :: i1,i2,j1,j2, i,j

   !! complete tiles
   do i1 =  1, 100,  4
   do j1 =  1,  96, 16
   do i2 = i1,  i1 + 3
   do j2 = j1,  j1 +15
      A(j2,i2) = A(j2,i2) + 1
   end do; end do; end do; end do

   !! partial tiles / remainder (not tiled)
   do i =  1, 100
   do j = 97, 100
      A(j,i) = A(j,i) + 1
   end do; end do

end subroutine
