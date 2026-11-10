! @@name:       fuse.4
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.1
subroutine fuse(n, A, B)
  implicit none
  integer, intent(in) :: n
  real, intent(out) :: A(n,*), B(n,*)
  integer :: i, j, k, l 
  real, external :: f, g

  !$omp fuse depth(2)
    do i = 1, n
      do k = 1, n
        A(k,i) = f(i,k)
      end do
    end do
    do j = 1, n
      do l = 1, n
        B(l,j) = g(j,l)
      end do
    end do
  !$omp end fuse
end subroutine

subroutine fuse_midpoint(n, A, B)
  implicit none
  integer, intent(in) :: n
  real, intent(out) :: A(n,*), B(n,*)
  integer :: ij, k, l 
  real, external :: f, g

  do ij = 1, n
    !$omp fuse depth(1)
      do k = 1, n
        A(k,ij) = f(ij,k)
      end do
      do l = 1, n
        B(l,ij) = g(ij,l)
      end do
    !$omp end fuse
  end do
end subroutine

subroutine fuse_equivalent(n, A, B)
  implicit none
  integer, intent(in) :: n
  real, intent(out) :: A(n,*), B(n,*)
  integer :: ij, kl 
  real, external :: f, g

  do ij = 1, n
    do kl = 1, n
      A(kl,ij) = f(ij,kl)
      B(kl,ij) = g(ij,kl)
    end do
  end do
end subroutine
