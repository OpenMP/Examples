! @@name:       pause_resource.2b
! @@type:       F-free
! @@operation:  link
! @@expect:     success
! @@version:	pre_omp_3.0
! This program compiles to an executable "subprogram"
subroutine compute(i, j, k, r)
   implicit none
   integer :: i, j, k
   real(8) :: r
   r = i + j + k
end subroutine compute

program subprogram
   implicit none
   integer :: i, j, k
   real(8) :: s, r
   integer, parameter :: n = 500

   write (*,*) 'In subprogram'
   s = 0.d0
!$omp parallel do private(r) reduction(+:s)
   do i = 1, n
      do j = 1, n
         do k = 1, n
            call compute(i, j, k, r)
            s = s + r
         end do
      end do
   end do
!$omp end parallel do

   write (*,*) 'Sum = ',s
end program subprogram
