! @@name:       loop.2f90
! @@type:       F-free, omp_5.0
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
program main
  integer, parameter :: N=100
  real :: x(N), y(N) 
  real :: a = 2.0e0

  x=[i,i=1,N]; y=1.0e0                    !! initialize

  !$omp parallel
    !$omp loop
       do i=1,N; y(i) = a*x(i) + y(i); enddo
  !$omp end parallel
      
  if(y(N) /= N*2.0e0) print*,"Error: 2*N /= y(N); y(N)=",y(N)
end program 
