! @@name: declare_variant.2f90
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success
! @@version: omp_5.0

module subs
  use omp_lib
contains

   subroutine base_saxpy(s,x,y)              !! base function
      real,intent(inout) :: s,x(:),y(:)
     !$omp  declare variant( avx512_saxpy ) &
     !$omp&           match( device={isa("core-avx512")} )

      y = s*x + y

   end subroutine

   subroutine avx512_saxpy(s,x,y)               !! function variant
      real,intent(inout) :: s,x(:),y(:)
      integer            :: i,n
      n=size(x)
                             !!assume 64-byte alignment for AVX-512
      !$omp parallel do simd simdlen(16) aligned(x,y: 64)
      do i = 1,n
         y(i) = s*x(i) + y(i)
      end do

   end subroutine

end module subs


program main
   use omp_lib
   use subs

   integer, parameter :: N=1000, align=64
   real, allocatable  :: x(:),y(:)
   real               :: s = 2.0e0
   integer            :: i 

   allocate(x(N),y(N))   !! Assumes allocation is 64-byte aligned
                         !! (using compiler options, or another
                         !! allocation method).

                         !! loc is non-standard, but found everywhere
                         !! remove these lines if not available
   if(modulo(loc(x),align) /= 0 .and. modulo(loc(y),align) /=0 ) then
      print*,"ERROR: x|y not 64-byte aligned"; stop
   endif

   do i=1,N  !! initialize
     x(i)=i 
     y(i)=i
   end do

   call base_saxpy(s,x,y)

   write(*,'("y(1),y(N):",2f6.0)') y(1),y(N) !!output: y... 3. 3000.

   deallocate(x,y)

end program
