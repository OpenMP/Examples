! @@name: affinity_display.3
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success
! @@version: omp_5.0
program affinity_display
   use omp_lib
   implicit none
   integer, parameter :: FORMAT_STORE=80
   integer, parameter :: BUFFER_STORE=80

   integer            :: i, n, thrd_num, nchars, max_req_store

   character(FORMAT_STORE)     :: default_format
   character(*), parameter     :: my_format = &
                                  "host=%20H thrd_num=%0.4n binds_to=%A"
   character(:), allocatable   :: buffer(:)
   character(len=0)            :: null


!  CODE SEGMENT 1         AFFINITY FORMAT

!                         Get and Display Default Affinity Format

   nchars = omp_get_affinity_format(default_format)
   print*,"Default Affinity Format: ", trim(default_format)

   if( nchars > FORMAT_STORE) then
      print*,"Caution: Reported Format is truncated.  Increase"
      print*,"         FORMAT_STORE to ", nchars
   endif

!                         Set Affinity Format

   call omp_set_affinity_format(my_format)
   print*,"Affinity Format set to: ", my_format


!  CODE SEGMENT 2         CAPTURE AFFINITY 

!                         Set up buffer for affinity of n threads

   n = omp_get_num_procs()
   allocate( character(len=BUFFER_STORE)::buffer(0:n-1) )

!                         Capture Affinity using Affinity Format set above.
!                         Use max reduction to check size of buffer areas
   max_req_store = 0
   !$omp parallel private(thrd_num,nchars) reduction(max:max_req_store)

      if(omp_get_num_threads()>n) stop "ERROR: increase buffer lines"

      thrd_num=omp_get_thread_num()
      nchars=omp_capture_affinity(buffer(thrd_num),null)
      if(nchars>max_req_store) max_req_store=nchars
      !  ...

   !$omp end parallel

   do i = 0, n-1
      print*, "thrd_num= ",i,"   affinity:", trim(buffer(i))
   end do
         !  For 4 threads with OMP_PLACES='{0,1},{2,3},{4,5},{6,7}'
         !  Format:   host=%20H thrd_num=%0.4n binds_to=%A

         !  affinity: host=hpc.cn567            thrd_num=0000 binds_to=0,1
         !  affinity: host=hpc.cn567            thrd_num=0001 binds_to=2,3
         !  affinity: host=hpc.cn567            thrd_num=0002 binds_to=4,5
         !  affinity: host=hpc.cn567            thrd_num=0003 binds_to=6,7

   if(max_req_store > BUFFER_STORE) then
      print*,  "Caution: Affinity string truncated.  Increase"
      print*,  "         BUFFER_STORE to ",max_req_store
   endif

   deallocate(buffer)
end program
