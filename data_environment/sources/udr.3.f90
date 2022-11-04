! @@name:	udr.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_4.0
program max_loc
   implicit none

   type :: mx_s
      real value
      integer index
   end type

   !$omp declare reduction(maxloc: mx_s: &
   !$omp&        mx_combine(omp_out, omp_in)) &
   !$omp&        initializer(mx_init(omp_priv, omp_orig))

   integer, parameter :: N = 100
   type(mx_s) :: mx
   real :: val(N), d
   integer :: i, count

   count = N
   do i = 1, count
      d = N*0.8 - i + 1
      val(i) = N * N - d * d
   enddo

   mx%value = val(1)
   mx%index = 1
   !$omp parallel do reduction(maxloc: mx)
   do i = 2, count
      if (mx%value < val(i)) then
         mx%value = val(i)
         mx%index = i
      endif
   enddo

   print *, 'max value = ', mx%value, ' index = ', mx%index
   ! prints 10000, 81

 contains

 subroutine mx_combine(out, in)
   implicit none
   type(mx_s), intent(inout) :: out
   type(mx_s), intent(in) :: in

   if ( out%value < in%value ) then
      out%value = in%value
      out%index = in%index
   endif
 end subroutine mx_combine

 subroutine mx_init(priv, orig)
   implicit none
   type(mx_s), intent(out) :: priv
   type(mx_s), intent(in) :: orig

   priv%value = orig%value
   priv%index = orig%index
 end subroutine mx_init

end program
