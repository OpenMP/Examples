! @@name:	taskloop_dep.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
subroutine process_work_a(n, A)
   implicit none
   integer :: n
   real :: A(*)
   integer :: i

   ! Dependences for taskloop iterations and across taskloops

   ! TL1 taskloop
   ! nogroup removes the implicit taskgroup
   !$omp taskloop nogroup
   do i = 2, n
      !$omp task_iteration depend(inout: A(i)) depend(in: A(i-1))
      A(i) = A(i) + A(i) * A(i-1)
   end do
   !$omp end taskloop

   ! TL2 taskloop + grainsize
   !$omp taskloop grainsize(strict: 4) nogroup 
   do i = 2, n
      !$omp task_iteration depend(inout: A(i)) depend(in: A(i-4)) &
      !$omp&               if (mod(i, 4) == 1 .or. i == n)
      A(i) = A(i) + A(i) * A(i-1)
   end do
   !$omp end taskloop

   ! T3 other task
   !$omp task depend(in: A(n))
   print *, "A(n) =", A(n)
   !$omp end task
end subroutine
