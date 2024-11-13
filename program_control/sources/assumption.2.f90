! @@name:       assumption.2
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0

module mm
  interface
    subroutine init(arr, n)
      integer :: arr(*)
      integer :: n
    end subroutine
    function element_transform(a) result(r)
      !$omp declare target
      integer :: a, r
    end function
  end interface
end module

program main
   use mm
   integer, parameter :: N=5
   integer  :: arr(N), arr_bang(N)

!!Case 1: Use in sequential code
   !$omp assume no_openmp
     call init(arr,N)
   !$omp end assume
    
!!Case 2: Use inside openmp construct
   !$omp target teams loop map(to: arr) map(from: arr_bang)
   do i=1,N
      !$omp assume no_parallelism
        arr_bang(i) = element_transform(arr(i))
      !$omp end assume
   enddo

   print *, arr_bang(1), arr_bang(N)

end program main