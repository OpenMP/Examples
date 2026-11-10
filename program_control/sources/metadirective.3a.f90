! @@name:	metadirective.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
module params
   integer, parameter          :: N=1000
   double precision, parameter :: M_PI=4.0d0*DATAN(1.0d0)
                                     ! 3.1415926535897932_8
end module

! This procedure is expected to be called from within a target teams
! or outside a target region from a parallel region and will
! distribute iterations across the league or team, respectively.
subroutine exp_pi_diff(d, my_pi)
  use params
  implicit none
  integer          ::  i
  double precision ::  d(N), my_pi
  !$omp declare target

  !$omp   metadirective &
  !$omp&  when( construct={target}: loop bind(teams) )  &
  !$omp&  otherwise( loop bind(parallel) )
  do i = 1,size(d)
     d(i) = exp( (M_PI-my_pi)*i )
  end do
end subroutine

program main
  ! Calculates sequence of exponentials: (M_PI-my_pi) * index
  ! M_PI is from usual way, and my_pi is user provided.
  ! Fortran Standard does not provide PI.

  use params
  implicit none
  double precision :: d(N)
  double precision :: my_pi=3.14159265358979d0

  ! Case 1: exp_pi_diff called from a target teams region, so
  ! that array is computed across league of teams. Note, however,
  ! that a target teams region that falls back to host execution
  ! may not work if the callee depends on the target trait.
  !$omp target teams map(from: d)
  call exp_pi_diff(d, my_pi)
  !$omp end target teams
  ! value should be near 1
  print*, "d(N) = ", d(N) ! 1.00000000000311

  ! Case 2: exp_pi_diff called from a parallel region, so that
  ! array is computed across team of threads.
  !$omp parallel
  call exp_pi_diff(d, my_pi) 
  !$omp end parallel
  ! value should be near 1
  print*, "d(N) = ", d(N) ! 1.00000000000311

end program
