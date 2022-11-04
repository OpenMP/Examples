! @@name:	mem_model.4b
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_3.1
subroutine flush_correct
  use omp_lib
  implicit none
  integer a, b, tmp
  integer myid

  a = 0; b = 0
  !$omp parallel private(myid,tmp) num_threads(2)
    myid = omp_get_thread_num()

    if ( myid == 0 ) then     ! thread 0
      !$omp atomic write
        b = 1
      !$omp flush(a,b)        ! flushes are ordered
      !$omp atomic read
        tmp = a
    else                      ! thread 1
      !$omp atomic write
        a = 1
      !$omp flush(a,b)        ! flushes are ordered
      !$omp atomic read
        tmp = b
    endif
    if ( tmp == 0 ) then      ! access by single thread
      !! protected section
    endif
  !$omp end parallel
end subroutine
