! @@name:       directive_syntax_F_free_comment.1.f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
  program main
     use omp_lib
     integer,parameter :: NT = 4
  
     !$omp parallel do num_threads(NT)                !DIR 1
     do i = 1,NT
       write(*,'("thrd no", i2)') omp_get_thread_num()
     end do
  
     !$omp  parallel do  &     !continue line         !DIR 2
     !$omp num_threads(NT)     !or !$omp&
     do i = 1,NT
       write(*,'("thrd no", i2)') omp_get_thread_num()
     end do
  
     !$omp parallel num_threads(NT)                   !DIR 3
     !$omp do                                         !DIR 4
     do i = 1,NT
        write(*,'("thrd no", i2)') omp_get_thread_num()
     end do
     !$omp end parallel

  end program

!     repeated 3 times, any order
!     OUTPUT: thrd no  0
!     OUTPUT: thrd no  1
!     OUTPUT: thrd no  2
!     OUTPUT: thrd no  3
