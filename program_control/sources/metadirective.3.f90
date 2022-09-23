! @@name: metadirective.3
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success
! @@version: omp_5.2
module params
   integer, parameter :: N=1000
   DOUBLE PRECISION, PARAMETER::M_PI=4.0d0*DATAN(1.0d0)
                                     ! 3.1415926535897932_8
end module


subroutine exp_pi_diff(d,    my_pi)
  use params
  implicit none
  integer          ::  i
  double precision ::  d(N), my_pi
  !$omp declare target

  !$omp   metadirective &
  !$omp&      when( construct={target}: distribute parallel do )  &
  !$omp&      otherwise(                parallel do simd )

  do i = 1,size(d)
     d(i) = exp( (M_PI-my_pi)*i )
  end do

end subroutine

program main
  ! Calculates sequence of exponentials: (M_PI-my_pi) * index
  ! M_PI is from usual way, and my_pi is user provided.
  ! Fortran Standard does not provide PI

  use params
  implicit none
  double precision   :: d(N)
  double precision   :: my_pi=3.14159265358979d0

      !$omp target teams map(from: d)
      call exp_pi_diff(d,my_pi)
      !$omp end target teams
                                  ! value should be near 1
      print*, "d(N) = ",d(N)      ! 1.00000000000311

      call exp_pi_diff(d,my_pi) ! value should be near 1
      print*, "d(N) = ",d(N)      ! 1.00000000000311

end program
