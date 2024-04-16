! @@name:       assumption.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_5.1

module m
  !$omp assumes no_parallelism
  interface
    subroutine fun(A, i)  
      implicit none       
      integer :: A(*),i   

    end subroutine
  end interface

end module

program main
   use m
   implicit none
   integer,allocatable :: A(:), B(:)
   integer             :: i, N
   real                :: rand_no

   call random_number(rand_no)   !! create random,
   N = (int(rand_no*5)+1)*16     !! runtime number multiple of 16

   allocate(A(N),B(N))           !! alloc space & initialize
   do i = 1, N
      A(i) = 0; B(i) = i 
   end do

!! Case 1: Delimited scope, see module interface
    
   !$omp target teams distribute parallel do map(tofrom: A)
   do i = 1, N
     call fun(A,i)
   end do
    
!! Case 2: Block associated

   !$omp assume holds (8*(N/8) == N .and. N>0)  !! N is multiple of 8
   !$omp simd    
   do i = 1, N
     A(i) = A(i) + B(i)
   end do
   !$omp end assume

end program
