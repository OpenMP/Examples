! @@name:	udr.2.f90
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

  !$omp declare reduction( min : point :  &
  !$omp&   omp_out = point(min( omp_out%x, omp_in%x ), &
  !$omp&                   min( omp_out%y, omp_in%y )) ) &
  !$omp&   initializer( omp_priv = point( HUGE(0), HUGE(0) ) )

  !$omp declare reduction( max : point :  &
  !$omp&   omp_out = point(max( omp_out%x, omp_in%x ), &
  !$omp&                   max( omp_out%y, omp_in%y )) ) &
  !$omp&   initializer( omp_priv = point( 0, 0 ) )

  type(point) :: minp = point( HUGE(0), HUGE(0) ), maxp = point( 0, 0 )
  integer :: i

  !$omp parallel do reduction(min: minp) reduction(max: maxp)
  do i = 1, n
     minp%x = min(minp%x, points(i)%x)
     minp%y = min(minp%y, points(i)%y)
     maxp%x = max(maxp%x, points(i)%x)
     maxp%y = max(maxp%y, points(i)%y)
  end do
  print *, "min = (", minp%x, minp%y, ")"
  print *, "max = (", maxp%x, maxp%y, ")"

end subroutine
