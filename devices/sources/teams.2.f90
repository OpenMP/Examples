! @@name:	teams.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
function dotprod(B,C,N, block_size, num_teams, block_threads) result(sum)
implicit none
    real    :: B(N), C(N), sum
    integer :: N, block_size, num_teams, block_threads, i, i0
    sum = 0.0e0
    !$omp target map(to: B, C) map(tofrom: sum)
    !$omp teams num_teams(num_teams) thread_limit(block_threads) &
    !$omp&  reduction(+:sum)
    !$omp distribute
       do i0=1,N, block_size
          !$omp parallel do reduction(+:sum)
          do i = i0, min(i0+block_size,N)
             sum = sum + B(i) * C(i)
          end do
       end do
    !$omp end teams
    !$omp end target
end function

! Note:  The variable sum is now mapped with tofrom, for correct 
! execution with 4.5 (and pre-4.5) compliant compilers. See Devices Intro.
