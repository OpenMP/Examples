! @@name:       flatten.4
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.1
subroutine flatten(A)
  implicit none
  real    :: A(128,128)
  integer :: i, j

  !$omp do schedule(static)
  !$omp flatten depth(2)
  do i = 1, 128
    do j = 1, i
      A(j,i) = 0;
    enddo
  enddo

end subroutine
