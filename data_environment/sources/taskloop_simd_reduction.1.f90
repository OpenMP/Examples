! @@name:	taskloop_simd_reduction.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.1
program main

  use omp_lib
  integer, parameter ::  N=100
  integer            :: i, a(N), asum=0

  a = [( i, i=1,N )]    !! initialize

!! taskloop reductions

  !$omp parallel masked
  !$omp taskloop reduction(+:asum)                  !! taskloop 1
    do i=1,N;  asum = asum + a(i);  enddo
  !$omp end taskloop
  !$omp end parallel masked


  !$omp parallel reduction(task, +:asum)            !! parallel reduction a

     !$omp masked
     !$omp task            in_reduction(+:asum)     !! task 2
       do i=1,N;  asum = asum + a(i);  enddo
     !$omp end task
     !$omp end masked

     !$omp masked taskloop in_reduction(+:asum)     !! taskloop 2
       do i=1,N;  asum = asum + a(i);  enddo
     !$omp end masked taskloop

  !$omp end parallel

!! taskloop simd reductions

  !$omp parallel masked
  !$omp taskloop simd reduction(+:asum)             !! taskloop simd 3
    do i=1,N;  asum = asum + a(i);  enddo
  !$omp end taskloop simd
  !$omp end parallel masked


  !$omp parallel reduction(task, +:asum)            !! parallel reduction b

    !$omp masked
    !$omp task                 in_reduction(+:asum) !! task 4
       do i=1,N;  asum = asum + a(i);  enddo
    !$omp end task
    !$omp end masked

    !$omp masked taskloop simd in_reduction(+:asum) !! taskloop simd 4
       do i=1,N;  asum = asum + a(i);  enddo
    !$omp end masked taskloop simd

  !$omp end parallel

  print*,"asum=",asum   !! output: asum=30300

end program
