! @@name:	loop.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
module xyz_data
  integer, parameter :: N=1024
  integer :: x(N,N)
  integer :: y(N), z(N)

contains 
  subroutine foo() 
    integer :: i, j

    !! i-loop distributed across encountering league of
    !! teams
    !$omp loop bind(teams)
    do i = 1, N
      !! this loop has an implicit bind(thread)
      !$omp loop
      do j = 1, N
        x(j,i) = x(j,i) + y(i)*z(i)
      end do
    end do
  end subroutine
end module

program main
 use xyz_data
 integer ::  error = 0

  do i = 1, N
    do j = 1, N
      x(j,i) = 0
    end do
  end do

  do i = 1, N
    y(i) = i
    z(i) = i + 1
  end do

  !$omp teams num_teams(4) 
  call foo()
  !$omp end teams

!!check values
  do i = 1, N
    do j = 1, N
      if( x(j,i) /= i * (i+1) ) then
        error = error + 1
      endif
    enddo
  enddo

  if(error .gt. 0) then
    print*, "FAILED"
    stop 1
  end if
  
  print*, "PASSED"

end program
