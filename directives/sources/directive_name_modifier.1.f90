! @@name:       directive_name_modifier.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine conditional_parallel(n, a, x, y, condition)
  implicit none
  integer, intent(in)    :: n
  real,    intent(in)    :: a, x(n)
  real,    intent(inout) :: y(n)
  logical, intent(in)    :: condition
  integer                :: i

  !! the if clause applies to all constituent directives that accept it
  !! (all-constituents is the default property), in this case the
  !! parallel directive
  !$omp parallel do if(condition)
  do i = 1,n 
    y(i) = a*x(i)+y(i)
  end do

  !! the if clause applies to the parallel directive only
  !$omp parallel do if(parallel: condition)
  do i = 1,n 
    y(i) = a*x(i)+y(i)
  end do

  !! the if clause applies to the parallel do constituent directive,
  !! which has the effect of only applying to the parallel leaf directive
  !$omp parallel do if(parallel do: condition)
  do i = 1,n 
    y(i) = a*x(i)+y(i)
  end do

  !! the if clause applies to the taskloop simd constituent directive,
  !! which has the effect of applying to both the taskloop and simd leaf
  !! directives
  !$omp parallel masked taskloop simd if(taskloop simd: condition)
  do i = 1,n 
    y(i) = a*x(i)+y(i)
  end do

end subroutine
