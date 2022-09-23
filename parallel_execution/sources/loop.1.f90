! @@name:       loop.1
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:    omp_5.0

program main
  integer, parameter :: N=100
  real :: x(N), y(N) 
  real :: a = 2.0e0

  x=(/ (i,i=1,N) /); y=1.0e0                    !! initialize

  !$omp parallel
    !$omp loop
       do i=1,N; y(i) = a*x(i) + y(i); enddo
  !$omp end parallel
      
  if(y(N) /= N*2.0e0) print*,"Error: 2*N /= y(N); y(N)=",y(N)
end program 
