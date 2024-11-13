! @@name:	induction.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
subroutine comp_poly(N, x, c)
   implicit none
   ! x:    input: value of x for which to eval the polynomial
   ! c(N): input: the coefficients
   integer :: N
   double precision :: x, c(*)

   double precision :: x0 = 1.0  ! initial value x^0 == 1
   double precision :: xi        ! x^i
   double precision :: result    ! accumulator for the result
   integer :: i

   !! Case 1: induction clause
   xi = x0
   result = 0.0
   !$omp parallel do reduction(+: result) induction(step(x),*: xi)
   do i = 1, N
      result = result + c(i) * xi
      xi = xi * x
   end do
   print *, 'C1: result =', result, ', xn =', xi

   !! Case 2: inscan reduction
   xi = x0
   result = 0.0
   !$omp parallel do reduction(+: result) reduction(inscan,*: xi)
   do i = 1, N
      result = result + c(i) * xi
      !$omp scan exclusive(xi)
      xi = xi * x
   end do
   print *, 'C2: result =', result, ', xn =', xi

   !! Case 3: closed form
   result = 0.0
   !$omp parallel do reduction(+: result) lastprivate(xi)
   do i = 1, N
      xi = x0 * (x ** (i-1))      ! induction operation in closed form
      result = result + c(i) * xi
      xi = xi * x
   end do
   print *, 'C3: result =', result, ', xn =', xi
end subroutine
