! @@name:	target_task_reduction.2a
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
program target_task_reduction_ex2
   interface
      subroutine device_compute(res)
      !$omp declare target enter(device_compute)
        integer :: res
      end subroutine device_compute
      subroutine host_compute(res)
        integer :: res
      end subroutine host_compute
   end interface
   integer :: sum
   sum = 0
   !$omp parallel sections reduction(task,+:sum)
      !$omp section
         !$omp target in_reduction(+:sum) nowait
           call device_compute(sum)
         !$omp end target
      !$omp section
         call host_compute(sum)
   !$omp end parallel sections
   print *, "sum = ", sum
   !!OUTPUT: sum = 2
end program

subroutine device_compute(sum)
   integer :: sum
   !$omp declare target enter(device_compute)
   sum = 1
end subroutine
subroutine host_compute(sum)
   integer :: sum
   sum = 1
end subroutine
