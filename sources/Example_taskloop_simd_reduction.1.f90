! @@name:	taskloop_simd_reduction.1f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.0

program main

  use omp_lib
  integer, parameter ::  N=100
  integer            :: i, a(N), asum=0

  a = [( i, i=1,N )]    !! initialize

!! taskloop reductions

  !$omp parallel master
  !$omp taskloop reduction(+:asum)                     !! taskloop 1
    do i=1,N;  asum = asum + a(i);  enddo
  !$omp end taskloop
  !$omp end parallel master


  !$omp parallel reduction(task, +:asum)               !! parallel reduction a

     !$omp master
     !$omp task            in_reduction(+:asum)        !! task 2
       do i=1,N;  asum = asum + a(i);  enddo
     !$omp end task
     !$omp end master

     !$omp master taskloop in_reduction(+:asum)        !! taskloop 2
       do i=1,N;  asum = asum + a(i);  enddo
     !$omp end master taskloop

  !$omp end parallel

!! taskloop simd reductions

  !$omp parallel master
  !$omp taskloop simd reduction(+:asum)                !! taskloop simd 3
    do i=1,N;  asum = asum + a(i);  enddo
  !$omp end taskloop simd
  !$omp end parallel master


  !$omp parallel reduction(task, +:asum)               !! parallel reduction b

    !$omp master
    !$omp task                 in_reduction(+:asum)    !! task 4
       do i=1,N;  asum = asum + a(i);  enddo
    !$omp end task
    !$omp end master

    !$omp master taskloop simd in_reduction(+:asum)    !! taskloop simd 4
       do i=1,N;  asum = asum + a(i);  enddo
    !$omp end master taskloop simd

  !$omp end parallel

  print*,"asum=",asum   !! output: asum=30300

end program
