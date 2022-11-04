! @@name:	metadirective.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
program main
   integer, parameter :: N= 100
   integer ::  v1(N), v2(N), v3(N);

   do i=1,N;  v1(i)=i; v2(i)=-i;  enddo   ! initialize

   !$omp  target map(to:v1,v2) map(from:v3) device(0)
   !$omp  metadirective &
   !$omp&     when(     device={arch("nvptx")}: teams loop) &
   !$omp&     otherwise(                     parallel loop)
     do i= 1,N; v3(i) = v1(i) * v2(i); enddo
   !$omp  end target

   print *, v3(1),v3(N) !!output: -1  -10000 
end program
