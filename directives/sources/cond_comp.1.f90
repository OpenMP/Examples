! @@name:	cond_comp.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	pre_omp_3.0
program example
!$ use omp_lib
   integer iam, index

   iam = 0
   index = 100

  ! the sentinel !$ can start on any column
  !$ iam = omp_get_thread_num() +   &
  !$&      index

!$ print *, "Compiled by an OpenMP-compliant implementation."
   print *, "iam = ", iam

end program example
