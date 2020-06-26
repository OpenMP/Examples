! @@name:       target_reverse_offload.1f
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version:    omp_5.0

!$omp requires reverse_offload

subroutine error_handler(wrong_value, index)
  integer :: wrong_value,index
  !$omp declare target device_type(host)

   write( *,'("Error in offload: A(",i3,")=",i3)' ) index,wrong_value
   write( *,'("       Expecting: A(  i)=  i")'    )
   stop
     !!output: Error in offload: A( 99)= -1
     !!               Expecting: A(  i)=  i
end subroutine
 
program rev_off
  use omp_lib
  integer, parameter :: N=100
  integer            :: A(N) = (/ (i, i=1,100) /)
 
   A(N-1)=-1
 
   !$omp target map(A)
      do i=1,N
         if (A(i) /= i)  then
           !$omp target device(ancestor: 1) map(always,to :A(i))
               call error_handler(A(i), i)
           !$omp end target
         endif
      end do
   !$omp end target
 
end program
