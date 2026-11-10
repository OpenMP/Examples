! @@name:	declare_variant.3
! @@type:	F-free
! @@operation:	view
! @@expect:	none
! @@version:	omp_6.0
program main

#ifdef _OPENMP
!$omp begin declare_variant match(device={kind(nohost)})

  ! NVIDIA vendor specializations
  !$omp begin declare_variant match(device={isa(sm_70)})
    use my_fun_nvidia_sm_70_mod
  !$omp end declare_variant
 
  !$omp begin declare_variant match(device={isa(sm_80)})
    use my_fun_nvidia_sm_80_mod
  !$omp end declare_variant

  ! AMD hardware specialization
  !$omp begin declare_variant match(device={arch(amdgcn)})
    use my_fun_amd_hip_mod
  !$omp end declare_variant

!$omp end declare_variant

!$omp begin declare_variant match(device={kind(host)})
  use my_fun_mod 
!$omp end declare_variant  
#else
  use my_fun_generic_mod
#endif

  implicit none

  integer, parameter :: N = 64
  real(8) :: array(N)
  integer :: i

  ! Array initialization
  do i = 1, N
    array(i) = 0.0d0
  end do

  !$omp target map(tofrom: array)
  do i = 1, N
    array(i) = my_fun(i-1)  
  end do
  !$omp end target

end program main
