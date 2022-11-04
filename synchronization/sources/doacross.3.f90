! @@name:	doacross.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	ct-error
! @@version:	omp_5.2
subroutine work_wrong(N, p)
  integer :: N
  real(8), dimension(N,N,N) :: p
  integer :: i, j, k
  real(8) :: tmp1, tmp2, tmp3

!$omp parallel do ordered(2) private(i,j,k,tmp1,tmp2,tmp3)
  do i=2, N-1
    do j=2, N-1
    !$omp ordered doacross(sink: i-1,j) doacross(sink: i+1,j) &
    !$omp&        doacross(sink: i,j-1) doacross(sink: i,j+1)
      do k=2, N-1
        tmp1 = p(k-1,j,i) + p(k+1,j,i)
        tmp2 = p(k,j-1,i) + p(k,j+1,i)
        tmp3 = p(k,j,i-1) + p(k,j,i+1)
        p(k,j,i) = (tmp1 + tmp2 + tmp3) / 6.0
      end do
! missing !$omp ordered doacross(source:)
    end do
  end do
end subroutine
