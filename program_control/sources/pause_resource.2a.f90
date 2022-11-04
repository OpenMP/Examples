! @@name:       pause_resource.2a
! @@type:       F-free
! @@operation:  link
! @@expect:     success
! @@version:    omp_5.0
program relinquish
   use omp_lib
   implicit none
   integer :: err

   write (*,*) 'In relinquish'

!$omp parallel
   write (*,*) 'In parallel region 1'
!$omp end parallel

   err = omp_pause_resource_all(omp_pause_soft)

   ! execute the external subprogram produced from pause_resource.2b
   call execute_command_line(command='./subprogram', wait=.true., &
                             cmdstat=err)
   if (err /= 0) write (*,*) 'Warning: subprogram failed to execute'

!$omp parallel
   write (*,*) 'In parallel region 2'
!$omp end parallel
end program relinquish
