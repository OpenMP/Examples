! @@name:	directive_syntax_F_block.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.1
  program main

     use omp_lib
     implicit none
     integer,parameter :: NT = 2, chunks=3
    
    !$omp parallel num_threads(NT)
     block                          ! Fortran 2008 OMP 5.1
       integer :: thrd_no,i
       thrd_no= omp_get_thread_num()
        !$omp do schedule(static,chunks)
        do i = 1,NT*chunks
           write(*,'("ndx=",i0.2," thrd_no=", i0.2)') i,thrd_no
        end do
     end block
  end program

! any order
! OUTPUT: ndx=01 thrd_no=00
! OUTPUT: ndx=02 thrd_no=00
! OUTPUT: ndx=03 thrd_no=00
! OUTPUT: ndx=04 thrd_no=01
! OUTPUT: ndx=05 thrd_no=01
! OUTPUT: ndx=06 thrd_no=01
