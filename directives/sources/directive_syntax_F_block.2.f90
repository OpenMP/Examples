! @@name:       directive_syntax_F_block.2
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:    omp_5.1
program main

   use omp_lib
   implicit none

  !$omp parallel num_threads(2)
     if( omp_get_thread_num() == 0 ) &
         print*, "Loosely  structured block  -- end required."
     block                                ! BLOCK Fortran 2008
       if( omp_get_thread_num() == 0 ) &
           print*, "                           --"
     end block
  !$omp end parallel 

  !$omp parallel num_threads(2)
     block 
       if( omp_get_thread_num() == 0 ) &
           print*, "Strictly structured block  -- end not required."
     end block
 !!$omp end parallel !is optional for strictly structured block 

  print*, "Sequential part"

  !$omp parallel num_threads(2)                      !outer parallel
     if( omp_get_thread_num() == 0 ) & 
         print*, "Outer, loosely  structured block."
     !$omp parallel num_threads(2)                   !inner parallel
        block
          if( omp_get_thread_num() == 0 ) &
          print*, "Inner, strictly structured block."
        end block
     !$omp end parallel
  !$omp end parallel
  ! Two end directives are required here.
  ! A single "!$omp end parallel" terminator will fail.
  ! 1st end directive is assumed to be for inner parallel construct.
  ! 2nd end directive applies to outer parallel construct.

end program

!OUTPUT, in order:
! Loosely  structured block  -- end required.
!                            --
! Strictly structured block  -- end not required.
! Sequential part
! Outer, loosely  structured block.
! Inner, strictly structured block.
! Inner, strictly structured block.
