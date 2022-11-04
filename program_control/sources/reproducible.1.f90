! @@name:	reproducible.1
! @@type:	F-free
! @@operation:	link
! @@expect:	success
! @@version:	omp_5.0
program main
   use omp_lib
   implicit none
   integer, parameter :: n = 1000
   integer :: v(n), u(n)
   integer :: i
   integer, save :: sum
   !$omp threadprivate(sum)

   !! no data dependences, so can execute concurrently
   !$omp parallel do order(concurrent)
   do i = 1, n
      u(i) = i
      v(i) = i
      v(i) = v(i) + u(i) * u(i)
   end do

   !! with data dependences, so cannot execute iterations
   !! concurrently with the order(concurrent) clause
   !$omp parallel do ordered
   do i = 2, n
      v(i) = v(i) + u(i) * u(i)
      !$omp ordered
	 v(i) = v(i) + v(i-1)
      !$omp end ordered
   end do

   sum = 0
   !! accessing a threadprivate variable, which would not be
   !! permitted if the order(concurrent) clause was present
   !$omp parallel do copyin(sum)
   do i = 2, n
      sum = sum + v(i)
   end do

   !$omp parallel
      print *,"sum = ",sum," on thread ", omp_get_thread_num()
   !$omp end parallel
   
end program
