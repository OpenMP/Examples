! @@name:	SIMD.6
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
function foo(p) result(r)
  implicit none
!$omp declare simd(foo) notinbranch
  integer :: p, r
  p = p + 10
  r = p
end function foo

function myaddint(a, b,  n) result(r)
  implicit none
  integer :: a(*), b(*), n, r
  integer :: i
  integer, external :: foo

  !$omp simd
  do i=1, n
      a(i) = foo(b(i))  ! foo is not called under a condition
  end do
  r = a(n)

end function myaddint

function goo(p) result(r)
  implicit none
!$omp declare simd(goo) inbranch
  real :: p, r
  p = p + 18.5
  r = p
end function goo

function myaddfloat(x, y, n) result(r)
  implicit none
  real :: x(*), y(*), r
  integer :: n
  integer :: i
  real, external :: goo

  !$omp simd
  do i=1, n
     if (x(i) > y(i)) then
        x(i) = goo(y(i))
        ! goo is called under the condition (or within a branch)
     else
        x(i) = y(i)
     endif
  end do

  r = x(n)
end function myaddfloat
