! @@name:	target_reverse_offload.7
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
subroutine error_handler(wrong_value, index)
  implicit none
  integer :: wrong_value,index
  !$omp requires reverse_offload
  !$omp declare target device_type(host)

   write( *,'("Error in offload: A(",i3,")=",i3)' ) index,wrong_value
   write( *,'("       Expecting: A(  i)=  i")'    )
   stop
     !!output: Error in offload: A( 99)= -1
     !!               Expecting: A(  i)=  i
end subroutine
 
program rev_off
  implicit none
  !$omp requires reverse_offload
  integer, parameter :: N=100
  integer            :: i
  integer            :: A(N) = (/ (i, i=1,100) /)
 
   A(N-1)=-1
 
   !$omp target map(A)
      do i=1,N
         if (A(i) /= i)  then
           !$omp target device(ancestor: 1) map(always,to: A(i))
               call error_handler(A(i), i)
           !$omp end target
         endif
      end do
   !$omp end target
 
end program
