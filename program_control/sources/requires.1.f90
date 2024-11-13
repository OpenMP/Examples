! @@name:	requires.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
module data
!$omp requires unified_shared_memory
  type,public :: mypoints
     double precision :: res
     double precision :: data(500)
  end type
end module

program main
  use data
  type(mypoints) :: p
  integer        :: q=0

  !$omp target    !! no map clauses needed
     q = q + 1    !! q is firstprivate
     call do_something_with_p(p,q)
  !$omp end target

  write(*,'(f5.0,i5)') p%res, q    !! output 1.   0

end program

subroutine do_something_with_p(p,q)
  use data
  !$omp declare target
  type(mypoints) :: p
  integer        :: q
 
  p%res = q;
  do i=1,size(p%data)
     p%data(i)=q*i
  enddo

end subroutine
