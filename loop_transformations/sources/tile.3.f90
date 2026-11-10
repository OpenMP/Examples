! @@name:       tile.3
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0

program main
  integer, parameter :: M=9, N=7
  integer            :: A(M,N)

  !$omp parallel do collapse(2) schedule(static) num_threads(4)
  !$omp tile sizes(grid: 2, 2)
  do  j = 1, N
  do  i = 1, M
    A(i,j) = i*100 + j 
  enddo
  enddo

  do  i = M, 1, -1
    write(*,'(I8)',advance="no")  A(i,1:N)
    write(*,'("")')
  enddo

end program
