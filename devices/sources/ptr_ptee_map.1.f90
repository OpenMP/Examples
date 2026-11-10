! @@name:	ptr_ptee_map.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
program main
  implicit none
  integer, pointer :: a(:)

  allocate(a(9))

  !$omp target_enter_data map(a) ! (A)

  call f1(a)
  ! Implementations may create a temporary descriptor for the call:
  !
  !   tmp_dv => a
  !   call f1(tmp_dv)
  !   a => tmp_dv

  !$omp target_enter_data map(ref_ptr, present: a) ! (B)
  !$omp target_exit_data map(delete: a)            ! (C)

  deallocate(a)

contains
  subroutine f1(x)
    implicit none
    integer, pointer :: x(:)

    !$omp target_enter_data map(ref_ptee, present: x) ! (D)
    !$omp target_enter_data map(present: x(:))        ! (E)
    !$omp target_enter_data map(ref_ptr, present: x)  ! (F), unspecified
  end subroutine
end program
