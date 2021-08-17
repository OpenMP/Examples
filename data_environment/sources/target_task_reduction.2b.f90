! @@name:	target_task_reduction.2b.f90
! @@type:	F-free
! @@compilable:	yes
! @@requires:	preprocessing
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_5.1
#if _OPENMP  < 202011
#define masked master
#endif

program target_task_reduction_ex2b
   interface
      subroutine device_compute(res)
      !$omp declare target to(device_compute)
        integer :: res
      end subroutine device_compute
      subroutine host_compute(res)
        integer :: res
      end subroutine host_compute
   end interface
   integer :: sum
   sum = 0
   !$omp parallel masked reduction(task,+:sum)
         !$omp target in_reduction(+:sum) nowait
           call device_compute(sum)
         !$omp end target
         call host_compute(sum)
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

