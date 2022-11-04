! @@name:	target_task_reduction.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
program target_task_reduction_ex1
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
   !$omp parallel masked
      !$omp taskgroup task_reduction(+:sum)
         !$omp target in_reduction(+:sum) nowait
            call device_compute(sum)
         !$omp end target
         !$omp task in_reduction(+:sum)
            call host_compute(sum)
         !$omp end task
      !$omp end taskgroup
   !$omp end parallel masked
   print *, "sum = ", sum
   !!OUTPUT: sum = 2
end program

subroutine device_compute(sum)
   integer :: sum
   sum = 1
end subroutine
subroutine host_compute(sum)
   integer :: sum
   sum = 1
end subroutine
