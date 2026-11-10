! @@name:       invalid_compound_names.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     ct-error
! @@version:    omp_6.0
subroutine component_names(n, x)
  implicit none
  integer, intent(in)    :: n
  real,    intent(inout) :: x(n)
  integer                :: i
  interface
    subroutine f()
    end subroutine
    subroutine g(xi)
      real :: xi
    end subroutine
  end interface

  !! invalid: repeated parallel disallowed
  !$omp parallel target parallel
    call f()
  !$omp end parallel target parallel

  !! invalid: target cannot be nested inside teams
  !$omp teams target
    call f()
  !$omp end teams target

  !! invalid: sections may only be appended to parallel
  !$omp teams sections
    call f()
  !$omp end teams sections

  !! invalid: masked may only be appended to parallel
  !$omp teams masked
    call f()
  !$omp end teams masked

  !! invalid: multiple data-mapping constituents
  !$omp target_data parallel target map(x(1:n))
    call f()
  !$omp end target_data parallel target

  !! invalid: task and taskloop both generate tasks in
  !! same parallel region
  !$omp task taskloop
  do i = 1, n
    call g(x(i))
  end do

end subroutine
