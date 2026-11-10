! @@name:       flatten.2
! @@type:       F-free
! @@operation:  run
! @@expect:     success
! @@version:    omp_6.1
module flat_subs
  implicit none
  integer, parameter :: Ni=3, Nj=5

contains  
  subroutine flatten_unroll(A)
    implicit none
    integer :: A(Nj,*)
    integer :: i, j

  !$omp unroll partial(4)
  !$omp flatten depth(2)
  do i = 1, Ni
    do j = 1, Nj
      A(j,i) = 0 
    enddo
  enddo
  end subroutine

  subroutine flatten_unroll_equivalent(A)
    implicit none
    integer, target  :: A(Nj,Ni)
    integer, pointer :: flat_A(:)
    integer          :: i, j
   
    flat_A(1:Nj*Ni) => A
    do i = 1, (Ni*Nj)/4

      flat_A((i-1)*4 + 1) = 0
      flat_A((i-1)*4 + 2) = 0
      flat_A((i-1)*4 + 3) = 0
      flat_A((i-1)*4 + 4) = 0
    enddo

    do i = ((Ni*Nj)/4)*4 + 1, Ni*Nj
      flat_A(i) = 0
    enddo

  end subroutine

end module flat_subs

program main
  use flat_subs
  implicit none
  integer :: A(Nj,Ni)

  call flatten_unroll(A)
  call flatten_unroll_equivalent(A)

end program
