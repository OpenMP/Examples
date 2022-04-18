! @@name:	lock_owner.1
! @@type:	F-fixed
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:    omp_5.1
        program lock
        use omp_lib
        integer :: x
        integer (kind=omp_lock_kind) :: lck

        call omp_init_lock (lck)
        call omp_set_lock(lck)
        x = 0

!$omp parallel shared (x)
!$omp masked
        x = x + 1
        call omp_unset_lock(lck)
!$omp end masked

!       Some more stuff.
!$omp end parallel

        call omp_destroy_lock(lck)

        end
