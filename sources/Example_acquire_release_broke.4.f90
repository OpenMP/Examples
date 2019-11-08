! @@name: acquire_release.4.f90
! @@type: F-free
! @@compilable: yes, omp_5.0
! @@linkable: yes
! @@expect: success

program rel_acq_ex4
   use omp_lib
   integer :: x, y, thrd
   integer :: tmp
   x = 0

!! !!! THIS CODE WILL FAIL TO PRODUCE CONSISTENT RESULTS !!!!!!!
!! !!! DO NOT PROGRAM SYNCHRONIZATION THIS WAY !!!!!!!

   !$omp parallel num_threads private(thrd) private(tmp)
      thrd = omp_get_thread_num()
      if (thrd == 0) then
         !$omp critical
         x = 10
         !$omp end critical
         ! an explicit flush directive that provides
         ! release semantics is needed here to
         ! complete the synchronization.
         !$omp atomic write
         y = 1
         !$omp end atomic
      else
         tmp = 0
         do while(tmp == 0)
           !$omp atomic read acquire ! or seq_cst
           tmp = x
           !$omp end atomic
         end do
         !$omp critical
         print *, "x = ", x  !! !! NOT ALWAYS 10
         !$omp end critical
      end if
   !$omp end parallel
end program
