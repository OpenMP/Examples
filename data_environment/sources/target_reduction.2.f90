! @@name:	target_reduction.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
program target_reduction_ex2
   interface
      function f(res)
             integer :: f, res
          end function
      function g(res)
             integer :: g, res
          end function
   end interface
   integer :: sum1, sum2, i
   integer, parameter :: n = 100
   sum1 = 0
   sum2 = 0
   !$omp target data map(sum1, sum2)
       !$omp target teams distribute reduction(+:sum1)
           do i=1,n
              sum1 = sum1 + f(i)
           end do
       !$omp target teams distribute map(sum1) reduction(+:sum2)
           do i=1,n
              sum2 = sum2 + g(i)*sum1
           end do
   !$omp end target data
   print *, "sum1 = ", sum1, ", sum2 = ", sum2
   !!OUTPUT: sum1 =     10100 , sum2 = 153015000
end program


integer function f(res)
   integer :: res
   f = res*2
end function
integer function g(res)
   integer :: res
   g = res*3
end function
