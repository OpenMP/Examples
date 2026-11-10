! @@name:       flatten.3
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.1
module flatten_subs
  implicit none
  integer,parameter :: Ni=128, Nj=128

contains
subroutine flatten(A)
  implicit none
  real    :: A(Nj,Ni)
  integer :: i, j

  !$omp flatten depth(2)
    do i = 1, Ni
      do j = 1, i
        A(j,i) = 0;
      enddo
    enddo

end subroutine
subroutine flatten_equivalent_empty_iterations(A)
  implicit none
  real    :: A(Nj,Ni)
  integer :: ij, i, j

    do ij = 1, Ni*Nj
      i = (ij - 1)/Nj + 1
      j = modulo(ij - 1, Nj) + 1
      if (j <= i) then
         A(j,i) = 0;
      else
        ! empty iteration
      end if
    enddo

end subroutine

end module

program main
  use flatten_subs
  implicit none
  real :: A(Nj,Ni)

  call flatten(A)
  call flatten_equivalent_empty_iterations(A)

end program
