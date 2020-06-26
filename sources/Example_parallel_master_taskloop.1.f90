! @@name:       parallel_master_taskloop.1.f
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:     omp_5.0

program main

   integer, parameter :: N=100
   integer :: i, a(N),b(N),c(N)

   do i=1,N                            !! initialize
      b(i) = i
      c(i) = i
   enddo

   !$omp parallel 
   !$omp master 
   !$omp taskloop                      !! taskloop 1
   do i=1,N
      a(i) = b(i) + c(i)
   enddo
   !$omp end taskloop
   !$omp end master 
   !$omp end parallel

   !$omp parallel master taskloop      !! taskloop 2
   do i=1,N
      b(i) = a(i) + c(i)
   enddo
   !$omp end parallel master taskloop

   !$omp parallel master taskloop simd !! taskloop 3
   do i=1,N
      c(i) = a(i) + b(i)
   enddo
   !$omp end parallel master taskloop simd

   print*,c(1),c(N)  !! 5 and 500

end program
