! @@name:	reproducible.2
! @@type:	F-free
! @@operation:	link
! @@expect:	success
! @@version:	omp_5.1
program main
   implicit none
   integer, parameter :: n = 1000
   integer :: v(n), u(n)
   integer :: i

   !$omp parallel 
      !! reproducible schedules are used the following two constructs 
      !$omp do order(reproducible: concurrent) 
      do i = 1, n
         u(i) = i
         v(i) = i
      end do
      !$omp end do nowait
      !$omp do order(reproducible: concurrent)
      do i = 1, n
         v(i) = v(i) + u(i) * u(i)
      end do 
   !$omp end parallel

   !$omp parallel 
      !! static schedules preserve data dependences between the loops
      !$omp do schedule(static) order(concurrent) 
      do i = 1, n
         u(i) = i
         v(i) = i
      end do 
      !$omp end do nowait
      !$omp do schedule(static) order(concurrent)
      do i = 1, n
         v(i) = v(i) + u(i) * u(i)
      end do
   !$omp end parallel

   !$omp parallel 
      !! the default reproducibility by the static schedule is not 
      !! preserved due to the unconstrained order clause.
      !! use of nowait here could result in data race.
      !$omp do schedule(static) order(unconstrained: concurrent)
      do i = 1, n
         u(i) = i
         v(i) = i
      end do
      !$omp do schedule(static) order(unconstrained: concurrent)
      do i = 1, n
         v(i) = v(i) + u(i) * u(i)
      end do
   !$omp end parallel
   
end program
