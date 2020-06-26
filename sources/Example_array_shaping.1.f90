! @@name: array_shaping.1.f90
! @@type: F-free
! @@compilable: yes
! @@linkable: no
! @@expect: success
! @@version: omp_5.0
module m
   interface
      subroutine do_work(a, nx, ny)
         !$omp declare target to(do_work)
         integer, intent(in) :: nx, ny
         double precision a(0:nx+1,ny)
      end subroutine do_work

      subroutine other_work(a, nx, ny)
         !$omp declare target to(other_work)
         integer, intent(in) :: nx, ny
         double precision a(0:nx+1,ny)
      end subroutine other_work

      subroutine exch_data(a, nx, ny)
         integer, intent(in) :: nx, ny
         double precision a(0:nx+1,ny)
      end subroutine exch_data
   end interface
end module m

subroutine array_shaping(a, nx, ny)
   use m
   implicit none
   integer, intent(in) :: nx, ny
   double precision a(0:nx+1,ny)

   ! map data to device and do work
   !$omp target data map(a)

      ! do work on the device
      !$omp target      ! map(a) is optional here
      call do_work(a, nx, ny)
      !$omp end target

      ! update boundary points (two rows of 2D array) on the host.
      ! data transferred are noncontiguous
      !$omp target update from( a(1,1:ny), a(nx,1:ny) )

      ! exchange ghost points with neighbors
      call exch_data(a, nx, ny)

      ! update ghost points (two rows of 2D array) on the device.
      ! data transferred are noncontiguous
      !$omp target update to( a(0,1:ny), a(nx+1,1:ny) )

      ! perform other work on the device
      !$omp target      ! map(a) is optional here
      call other_work(a, nx, ny)
      !$omp end target

   !$omp end target data

end subroutine
