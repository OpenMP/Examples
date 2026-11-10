! @@name:       dyn_groupprivate.1
! @@type:       F-free
! @@operation:  run
! @@expect:     success
! @@version:    omp_6.1
module kernels
  use, intrinsic :: iso_c_binding, only : c_ptr, c_sizeof, c_f_pointer
  use omp_lib
  implicit none

contains

  subroutine init(arr, n, tid)
    integer, intent(out) :: arr(*)
    integer, intent(in) :: n, tid
    !$omp declare_target
    integer :: i

    !$omp do
    do i = 1, n
      arr(i) = tid + i
    end do
    !$omp end do

  end subroutine init

  subroutine foo(sum, tid, N, M)
    integer, intent(inout) :: sum
    integer, intent(in)    :: tid, N, M
    !$omp declare_target

    integer, pointer :: x(:)
    integer, pointer :: y(:)
    type(c_ptr) :: cx, cy
    integer :: i

    cx = omp_get_dyn_gprivate_ptr(0, omp_access_cgroup)
    cy = omp_get_dyn_gprivate_ptr(N*c_sizeof(i), omp_access_cgroup)

    call c_f_pointer(cx, x, shape=[N])
    call c_f_pointer(cy, y, shape=[M])

    call init(x, N, tid)
    call init(y, M, tid)

    !$omp do reduction(+: sum)
    do i = 1, N
      sum = sum + x(i)
    end do
    !$omp end do

    !$omp do reduction(+: sum)
    do i = 1, M
      sum = sum + y(i)
    end do
    !$omp end do

  end subroutine foo

end module kernels

program main
  use omp_lib
  use kernels
  use, intrinsic :: iso_c_binding, only : c_sizeof
  implicit none

  integer, parameter :: NUM_TEAMS  = 4
  integer, parameter :: THREAD_LIM = 100

  integer, parameter :: N = 100, M = 5
  integer :: sums(NUM_TEAMS)
  integer :: total
  integer :: i
  integer, parameter :: int_bytes  = c_sizeof(i)   

  sums = 0

  !$omp target teams                                                 &
  !$omp&    num_teams(NUM_TEAMS)                                     &
  !$omp&    thread_limit(THREAD_LIM)                                 &
  !$omp&    dyn_groupprivate(N * int_bytes + M * int_bytes)
 
  !$omp parallel
    call foo(sums(omp_get_team_num() + 1), &
             omp_get_team_num(),           &
             N, M)
  !$omp end parallel

  !$omp end target teams

  total = 0
  do i = 1, NUM_TEAMS
    total = total + sums(i)
  end do

  write(*,'(A,I0)') 'Result: ', total

end program main
