! @@name: acquire_release.1.f90
! @@type: F-free
! @@compilable: yes, omp_5.0
! @@linkable: yes
! @@expect: success

program rel_acq_ex1
   use omp_lib
   integer :: x, y, thrd, tmp
   x = 0
   y = 0
   !$omp parallel num_threads(2) private(thrd, tmp)
      thrd = omp_get_thread_num()
      if (thrd == 0) then
         x = 10
         !$omp critical
         y = 1
         !$omp end critical
      else
         tmp = 0
         do while (tmp == 0)
            !$omp critical
            tmp = y
            !$omp end critical
         end do
         print *, "x = ", x  !! always "x = 10"
      end if
   !$omp end parallel
end program
