! @@name:	induction.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
module udi
  integer, parameter :: I2 = selected_int_kind(3)  ! enough for 256
  type Point
    real x, y, m
    integer(I2) color
   contains
    procedure initPoint, nextPoint
  end type

  !$omp declare induction(next : (Point, real))                  &
  !$omp&        inductor (omp_var = omp_var%nextPoint(omp_step)) &
  !$omp&        collector(omp_step * omp_idx)

 contains
  subroutine initPoint(this, x1, y1, m1)
    implicit none
    class(Point) this
    real x1, y1, m1
    this%x = x1; this%y = y1; this%m = m1
    this%color = mod(int(x1+y1), 256)
  end subroutine

  function nextPoint(this, distance) result(NewPoint)
  ! return a Point that is 'distance' away along slope m in the x direction
    implicit none
    class(Point) this
    real distance
    type(Point) NewPoint

    real deltaX, deltaY
    deltaX = distance/(sqrt(1.0 + this%m * this%m))
    deltaY = this%m * deltaX
    call NewPoint%initPoint(this%x+deltaX, this%y+deltaY, this%m)
  end function
end module

subroutine processPointsInLine(Start, NumberOfPoints, Separation)
  use udi
  implicit none
  type(Point) Start
  integer NumberOfPoints
  real Separation
  type(Point) P
  integer i

  P = Start
  !$omp parallel do induction(step(Separation), next : P)
  do i = 1, NumberOfPoints
    call process(P)
    P = P%nextPoint(Separation)
  end do
end subroutine

program main
  use udi
  implicit none
  type(Point) Start

  call Start%initPoint(1.0, -2.0, 0.5)
  call processPointsInLine(Start, 100, 0.25)
end program
