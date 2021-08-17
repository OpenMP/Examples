! @@name:	udr.1.f90
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_4.0
module data_type

  type :: point
    integer :: x
    integer :: y
  end type

end module data_type

subroutine find_enclosing_rectangle ( n, points )
  use data_type
  implicit none
  integer :: n
  type(point) :: points(*)

  !$omp declare reduction(min : point : minproc(omp_out, omp_in)) &
  !$omp&  initializer( omp_priv = point( HUGE(0), HUGE(0) ) )

  !$omp declare reduction(max : point : maxproc(omp_out, omp_in)) &
  !$omp&  initializer( omp_priv = point( 0, 0 ) )

  type(point) :: minp = point( HUGE(0), HUGE(0) ), maxp = point( 0, 0 )
  integer :: i

  !$omp parallel do reduction(min:minp) reduction(max:maxp)
  do i = 1, n
     call minproc(minp, points(i))
     call maxproc(maxp, points(i))
  end do
  print *, "min = (", minp%x, minp%y, ")"
  print *, "max = (", maxp%x, maxp%y, ")"

 contains
  subroutine minproc ( out, in )
    implicit none
    type(point), intent(inout) :: out
    type(point), intent(in) :: in

    out%x = min( out%x, in%x )
    out%y = min( out%y, in%y )
  end subroutine minproc

  subroutine maxproc ( out, in )
    implicit none
    type(point), intent(inout) :: out
    type(point), intent(in) :: in

    out%x = max( out%x, in%x )
    out%y = max( out%y, in%y )
  end subroutine maxproc

end subroutine
