! @@name:	fort_shared_var.1
! @@type:	F-free
! @@operation:	run
! @@expect:	undefined
! @@version:	pre_omp_3.0
program fort_shared_var
  implicit none
  integer, parameter :: N = 100
  integer a(N)
  integer i
  interface
    subroutine sub1(b)
      integer b(:)
    end subroutine
    subroutine sub2(b)
      integer, contiguous :: b(:)
    end subroutine
  end interface

  a = [(i, i=1,N)]
  !$omp parallel shared(a) num_threads(2)
    call sub1(a(::2))         ! copy-in/copy-out may or may not occur
  !$omp end parallel
  print *, 'sum1 =', sum(a)   ! sum1 may/may not be well defined

  a = [(i, i=1,N)]
  !$omp parallel shared(a) num_threads(2)
    call sub2(a(::2))         ! copy-in/copy-out result in a data race
  !$omp end parallel
  print *, 'sum2 =', sum(a)   ! sum2 is not well defined
end

subroutine sub1(b)
  implicit none
  integer b(:)
  integer i
  do i = 1, size(b)
    !$omp atomic
    b(i) = b(i) + 1
  end do
end subroutine

subroutine sub2(b)
  implicit none
  integer, contiguous :: b(:)
  integer i
  do i = 1, size(b)
    !$omp atomic
    b(i) = b(i) + 1
  end do
end subroutine
