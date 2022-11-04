! @@name:	parallel_masked_taskloop.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.1
program main

   integer, parameter :: N=100
   integer :: i, a(N),b(N),c(N)

   do i=1,N                            !! initialize
      b(i) = i
      c(i) = i
   enddo

   !$omp parallel 
   !$omp masked
   !$omp taskloop                      !! taskloop 1
   do i=1,N
      a(i) = b(i) + c(i)
   enddo
   !$omp end taskloop
   !$omp end masked
   !$omp end parallel

   !$omp parallel masked taskloop      !! taskloop 2
   do i=1,N
      b(i) = a(i) + c(i)
   enddo
   !$omp end parallel masked taskloop

   !$omp parallel masked taskloop simd !! taskloop 3
   do i=1,N
      c(i) = a(i) + b(i)
   enddo
   !$omp end parallel masked taskloop simd

   print*,c(1),c(N)  !! 5 and 500

end program
