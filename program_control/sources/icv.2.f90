! @@name:	icv.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
! @@env:	OMP_NUM_THREADS="4,5,6"
! @@env:	OMP_MAX_ACTIVE_LEVELS=3
subroutine prn_info(level)
   use omp_lib, only : omp_get_max_threads
   implicit none
   integer level

   !$omp masked
      print 10, level, omp_get_max_threads()
   !$omp end masked
   10 format("LV",i0,": nthrs_next=",i0)
end subroutine

program main
   implicit none

   !! run with OMP_NUM_THREADS="4,5,6" OMP_MAX_ACTIVE_LEVELS=3
   !! nthreads-var: 4,5,6
   !! max-active-levels-var: 3

   !! Case 1
   !$omp parallel       ! request 4 threads
      call prn_info(1)  ! LV1: nthrs_next=5

      !! nthreads-var: 5,6
      !$omp parallel       ! request 5 threads
         call prn_info(2)  ! LV2: nthrs_next=6

         !! nthreads-var: 6
         !$omp parallel       ! request 6 threads
            call prn_info(3)  ! LV3: nthrs_next=6

            !! nthreads-var: 6
         !$omp end parallel
      !$omp end parallel
   !$omp end parallel

   !! Case 2
   !$omp parallel num_threads(8)
      call prn_info(1)  ! LV1: nthrs_next=5

      !! nthreads-var: 5,6
      !$omp parallel       ! request 5 threads
         call prn_info(2)  ! LV2: nthrs_next=6

         !! nthreads-var: 6
         !$omp parallel       ! request 6 threads
            call prn_info(3)  ! LV3: nthrs_next=6

            !! nthreads-var: 6
         !$omp end parallel
      !$omp end parallel
   !$omp end parallel

   !! Case 3
   !$omp parallel num_threads(8,2)
      call prn_info(1)  ! LV1: nthrs_next=2

      !! nthreads-var: 2
      !$omp parallel       ! request 2 threads
         call prn_info(2)  ! LV2: nthrs_next=2

         !! nthreads-var: 2
         !$omp parallel       ! request 2 threads
            call prn_info(3)  ! LV3: nthrs_next=2

            !! nthreads-var: 2
         !$omp end parallel
      !$omp end parallel
   !$omp end parallel

end program
