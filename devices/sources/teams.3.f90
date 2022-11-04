! @@name:	teams.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.5
function dotprod(B,C,N) result(sum)
   real    :: B(N), C(N), sum
   integer :: N, i
   sum = 0.0e0
   !$omp target teams map(to: B, C)  &
   !$omp&             defaultmap(tofrom:scalar) reduction(+:sum)
   !$omp distribute parallel do reduction(+:sum)
      do i = 1,N
         sum = sum + B(i) * C(i)
      end do
   !$omp end target teams
end function

! Note:  The variable sum is now mapped with tofrom from the defaultmap
!  clause on the combined target teams construct, for correct 
!  execution with 4.5 (and pre-4.5) compliant compilers. See Devices Intro.
