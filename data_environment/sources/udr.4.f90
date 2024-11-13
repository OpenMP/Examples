! @@name:	udr.4
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
module data_red
! Declare data type.
  type dt
    real :: r1
    real :: r2
  end type

! Declare the user-defined operator .add.
  interface operator(.add.)
    module procedure addc
  end interface

! Declare the user-defined reduction operator .add.
!$omp declare reduction(.add. : dt)  &
!$omp&  combiner( omp_out=omp_out.add.omp_in ) &
!$omp&  initializer( dt_init(omp_priv) )

 contains
! Declare the initialization routine.
  subroutine dt_init(u)
    type(dt) :: u
    u%r1 = 0.0
    u%r2 = 0.0
  end subroutine

! Declare the specific procedure for the .add. operator.
  function addc(x1, x2) result(xresult)
    type(dt), intent(in) :: x1, x2
    type(dt) :: xresult
    xresult%r1 = x1%r1 + x2%r2
    xresult%r2 = x1%r2 + x2%r1
  end function

end module data_red

program main
  use data_red, only : dt, dt_init, operator(.add.)

  type(dt) :: xdt1, xdt2
  integer :: i

  xdt1 = dt(1.0,2.0)
  xdt2 = dt(2.0,3.0)

! The reduction operation
!$omp parallel do reduction(.add.: xdt1)
  do i = 1, 10
    xdt1 = xdt1 .add. xdt2
  end do
!$omp end parallel do

  print *, xdt1

end program
