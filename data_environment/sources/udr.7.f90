! @@name:	udr.7
! @@type:	F-free
! @@operation:	show
! @@expect:	success
! @@version:	omp_6.0
interface
   subroutine procA(out, in)                ! scalar
      real, intent(inout) :: out
      real, intent(in) :: in
   end subroutine
end interface
!$omp declare_reduction(addA: real) combiner(procA(omp_out, omp_in))

interface
   elemental subroutine procB(out, in)      ! elemental
      real, intent(inout) :: out
      real, intent(in) :: in
   end subroutine
end interface
!$omp declare_reduction(addB: real) combiner(procB(omp_out, omp_in))

interface procC
   elemental subroutine proc_elem(out, in)  ! elemental
      real, intent(inout) :: out
      real, intent(in) :: in
   end subroutine
   subroutine proc_array(out, in)           ! array
      real, intent(inout) :: out(100)
      real, intent(in) :: in(100)
   end subroutine
end interface
!$omp declare_reduction(addC: real) combiner(procC(omp_out, omp_in))

real :: a(100)

!$omp parallel reduction(addA: a)    ! Reduction A (array reduction)
   ! ...
!$omp end parallel

!$omp parallel reduction(addB: a)    ! Reduction B (array reduction)
   ! ...
!$omp end parallel

!$omp parallel reduction(addC: a)    ! Reduction C (array reduction)
   ! ...
!$omp end parallel
