! @@name:	task_detach.1f90
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:    omp_5.0
program main
  use omp_lib
  implicit none

  external :: async_work, work

  logical :: async=.true.
  integer(omp_event_handle_kind) :: event

  !$omp parallel
  !$omp masked

    !$omp task detach(event)

      if(async) then
        call async_work(omp_fulfill_event, event)
      else
        call work()
        call omp_fulfill_event(event)
      endif

    !$omp end task
                  !! Other work

    !$omp taskwait

  !$omp end masked
  !$omp end parallel

end program
