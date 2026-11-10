! @@name:       flatten.1
! @@type:       F-free
! @@operation:  run
! @@expect:     success
! @@version:    omp_6.1
module flat_subs
  use, intrinsic :: iso_fortran_env, only : int64
  implicit none
  integer, parameter :: Ni=3, Nj=4, Nk=3

contains  
  subroutine flatten(A)
    implicit none
    integer :: A(Nk,Nj,*)
    integer :: i, j, k

    !$omp flatten depth(2)
    do i = 1, Ni
      do j = 1, Nj
        do k = 1, Nk
          A(k,j,i) = 0
        enddo
      enddo
    enddo
  end subroutine
  
  subroutine flatten_equivalent(A)
    implicit none
    integer :: A(Nk,Nj,*)
    integer :: i, j, k
    integer(kind=int64) :: ij, tc_end

    tc_end = int(Ni, kind=int64) * int(Nj, kind=int64)
    do ij = 1, tc_end 
      i =       (ij-1)/Nj  + 1
      j = modulo(ij-1, Nj) + 1
      do k = 1, Nk
        A(k,j,i) = 0
      enddo
    enddo
  end subroutine

end module flat_subs 
  
program main
 use flat_subs
 implicit none
 integer :: A(Nk,Nj,Ni)

 call flatten(A)
 call flatten_equivalent(A)

end program
