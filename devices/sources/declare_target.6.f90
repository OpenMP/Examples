! @@name: declare_target.6.f90
! @@type: F-free
! @@compilable: yes
! @@linkable: no
! @@expect: success
! @@version: omp_4.5
module m_dat
   integer, parameter :: N=100000000
   !$omp declare target link(sp,sv1,sv2)
   real :: sp(N), sv1(N), sv2(N)

   !$omp declare target link(dp,dv1,dv2)
   double precision :: dp(N), dv1(N), dv2(N)

contains
   subroutine s_vec_mult_accum()
   !$omp declare target
      integer :: i

      !$omp parallel do
      do i = 1,N
        sp(i) = sv1(i) * sv2(i)
      end do

   end subroutine s_vec_mult_accum

   subroutine d_vec_mult_accum()
   !$omp declare target
      integer :: i

      !$omp parallel do
      do i = 1,N
        dp(i) = dv1(i) * dv2(i)
      end do

   end subroutine
end module m_dat

program prec_vec_mult
   use m_dat
 
   call s_init(sv1, sv2, N)
   !$omp target map(to:sv1,sv2) map(from:sp)
     call s_vec_mult_accum()
   !$omp end target
   call s_output(sp, N)
 
   call d_init(dv1, dv2, N)
   !$omp target map(to:dv1,dv2) map(from:dp)
     call d_vec_mult_accum()
   !$omp end target
   call d_output(dp, N)
 
end program
