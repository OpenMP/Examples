! @@name:	get_wtime.1
! @@type:	F-free
! @@operation:	link
! @@expect:	success
! @@version:	pre_omp_3.0
subroutine work_to_be_timed
  use, intrinsic :: iso_c_binding, only: c_int
  interface
     subroutine fsleep(sec) bind(C, name="sleep")
       import c_int
       integer(c_int), value :: sec
     end subroutine
  end interface
  call fsleep(2)
end subroutine

program do_work
  use omp_lib
  implicit none
  double precision :: start, end

  start = omp_get_wtime()
  call work_to_be_timed    ! any parallel or serial codes
  end = omp_get_wtime()

  print *, "Work took", end - start, "seconds"
  print *, "Precision of the timer is", omp_get_wtick(), "(sec)"
end program
