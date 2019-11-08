! @@name: affinity_display.1.f90
! @@type: F-free
! @@compilable: yes, omp_5.0
! @@linkable: yes
! @@expect: success
program affinity_display        ! MAX threads = 8, single socket system

   use omp_lib
   implicit none
   character(len=0) :: null

   call omp_display_affinity(null) !API call- Displays Affinity of Master Thread

! API CALL OUTPUT (default format): 
! team_num= 0, nesting_level= 0, thread_num= 0, thread_affinity= 0,1,2,3,4,5,6,7


                       ! OMP_DISPLAY_AFFINITY=TRUE, OMP_NUM_THREADS=8

   !$omp parallel num_threads(omp_get_num_procs())

     if(omp_get_thread_num()==0) then
        print*, "1st Parallel Region  -- Affinity Reported"
     endif

          ! DISPLAY OUTPUT (default format) has been sorted:
          ! team_num= 0, nesting_level= 1, thread_num= 0, thread_affinity= 0
          ! team_num= 0, nesting_level= 1, thread_num= 1, thread_affinity= 1
          ! ...
          ! team_num= 0, nesting_level= 1, thread_num= 7, thread_affinity= 7

      ! doing work here

   !$omp end parallel

   !$omp parallel num_threads( omp_get_num_procs() )
   
     if(omp_get_thread_num()==0) then
        print*, "Same Affinity in Parallel Region -- no Affinity Reported"
     endif

          ! NO AFFINITY OUTPUT:  
          !(output in 1st parallel region only for OMP_DISPLAY_AFFINITY=TRUE)
     
      ! doing more work here

   !$omp end parallel

                       ! Report Affinity for 1/2 number of threads
   !$omp parallel num_threads( omp_get_num_procs()/2 )

     if(omp_get_thread_num()==0) then
        print*, "Different Affinity in Parallel Region -- Affinity Reported"
     endif
    
         ! DISPLAY OUTPUT (default format) has been sorted:
         ! team_num= 0, nesting_level= 1, thread_num= 0, thread_affinity= 0,1
         ! team_num= 0, nesting_level= 1, thread_num= 1, thread_affinity= 2,3
         ! team_num= 0, nesting_level= 1, thread_num= 2, thread_affinity= 4,5
         ! team_num= 0, nesting_level= 1, thread_num= 3, thread_affinity= 6,7

      ! do work

   !$omp end parallel

end program
