! @@name:	atomic.4
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.0
subroutine boxster(box_totals, vals, box, N)
  use omp_lib
  external calc_val
  real,    intent(inout) :: box_totals(:)
  real,    intent(in)    :: vals(:)
  integer, intent(in)    :: box(:)
  integer                :: N, idx

    !$omp parallel do
    do idx=1,N
      call calc_val(vals(idx))
      !$omp atomic hint(omp_sync_hint_uncontended)
      box_totals( box(idx) ) = box_totals( box(idx) ) + vals(idx)
    enddo

end subroutine
