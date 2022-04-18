! @@name:       directive_syntax_F_fixed_comment.1
! @@type:       F-fixed
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
      program main
      include 'omp_lib.h'
      integer NT

      NT =4

c     sentinel c$omp or *$omp can also be used

c$omp parallel do num_threads(NT) !comments allowed here   DIR 1
      do i = 1,NT
        write(*,'("thrd no", i2)') omp_get_thread_num()
      end do

!$omp parallel do
!$omp+ num_threads(NT)          !cont. w. char in col. 6   DIR 2
      do i = 1,NT
        write(*,'("thrd no", i2)') omp_get_thread_num()
      end do

*$omp parallel num_threads(NT)  !multi-directive form      DIR 3
*$omp do                        !                          DIR 4
      do i = 1,NT
         write(*,'("thrd no", i2)') omp_get_thread_num()
      end do
*$omp end parallel
      end
!     repeated 3 times, any order
!     OUTPUT: thrd no  0
!     OUTPUT: thrd no  1
!     OUTPUT: thrd no  2
!     OUTPUT: thrd no  3
