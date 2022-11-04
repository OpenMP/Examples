! @@name:	tile.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.1
subroutine func1(A)
    integer :: A(128,100)
    integer :: i, j
    !$omp parallel do
    !$omp tile sizes(5,16)
    do i = 1, 100
    do j = 1, 128
        A(j,i) = j*1000 + i
    end do; end do
end subroutine

subroutine func2(A)
    integer :: A(128,100)
    integer :: i1, j1, i2, j2
    !$omp parallel do
    do i1 = 1, 100,5
    do j1 = 1, 128,16
       do i2 = i1, i1+( 5-1)
       do j2 = j1, j1+(16-1)
          A(j2,i2) = j2*1000 + i2
       end do; end do
    end do; end do
end subroutine
