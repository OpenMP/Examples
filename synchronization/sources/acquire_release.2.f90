! @@name:	acquire_release.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
program rel_acq_ex2
   use omp_lib
   integer :: x, y, thrd, tmp
   x = 0
   y = 0
   !$omp parallel num_threads(2) private(thrd, tmp)
      thrd = omp_get_thread_num()
      if (thrd == 0) then
         x = 10
         !$omp atomic write release ! or seq_cst
          y = 1
         !$omp end atomic
      else
         tmp = 0
         do while (tmp == 0)
            !$omp atomic read acquire ! or seq_cst
            tmp = y
            !$omp end atomic
         end do
         print *, "x = ", x  !! always "x = 10"
      end if
   !$omp end parallel
end program
