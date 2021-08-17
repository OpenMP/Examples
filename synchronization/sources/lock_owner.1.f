! @@name:	lock_owner.1f
! @@type:	F-fixed
! @@compilable:	yes
! @@requires:	preprocessing
! @@linkable:	yes
! @@expect:	success
! @@version:    omp_5.1
#if _OPENMP  < 202011
#define masked master
#endif

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
