! @@name:	declare_target_indirect_call.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
module funcs
  implicit none 

  interface 
    function func() result(i)
      integer :: i
    end function
  end interface

 contains
  function fun1() result(i)
  !$omp declare target enter(fun1) indirect !! indirect defaults to true
    integer :: i
    i=1
    return
  end function

  function fun2() result(i)
  !$omp declare target enter(fun2) indirect !! indirect defaults to true
    integer :: i
    i=2
    return
  end function

end module

program main
  use funcs
  implicit none
  procedure (func), pointer :: fptr=>null()
  integer :: ret_val=0, choice=0
  real    :: rand_no

  call  random_number(rand_no)  !! create random ( [0.0 - 1.0) )
  choice = nint(rand_no)+1      !! runtime number 1 or 2

  if (choice == 1 ) fptr => fun1
  if (choice == 2 ) fptr => fun2

  !$omp target map(from: ret_val)
     ret_val = fptr()  !! ret_val = 1/2 from fun1/fun2
  !$omp end target

  if (ret_val /= choice) then
     print*, "FAILED"; error stop 1 
  endif

end program
