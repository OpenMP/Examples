! @@name:       tile.2f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.1

subroutine func1(A)
    integer :: A(128,100)
    integer :: i, j
    !omp tile sizes(4, 4)
    !omp tile sizes(5,16)
    do i = 1, 100
    do j = 1, 128
       A(j,i) = j*1000 + i
    end do; end do
end subroutine

subroutine func2(A)
    integer :: A(128,100)
    integer :: i1, j1, i2, j2
    !omp tile sizes(4,4)
    do i1 = 1, 100,5
    do j1 = 1, 128,16
       do i2 = i1, i1+( 5-1)
       do j2 = j1, j1+(16-1)
          A(j2,i2) = j2*1000 + i2
       end do; end do
    end do; end do

end subroutine

subroutine func3(A)
    integer :: A(128,100)
    integer :: i11, j11, i12, j12, i2, j2
    do i11 = 1, 100,  5*4
    do j11 = 1, 128, 16*4
       do i12 = i11, i11+( 5*4-1),  5
       do j12 = j11, j11+(16*4-1), 16
          do i2 = i12, i12+ 5-1
          do j2 = j12, j12+16-1
             A(j2,i2) = j2*1000 + i2
          enddo; enddo; 
       enddo; enddo; 
    enddo; enddo

end subroutine
