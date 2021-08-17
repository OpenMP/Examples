! @@name: metadirective.4f90
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success
! @@version: omp_5.1

subroutine foo(a, n, use_gpu)
   integer :: n, a(n)
   logical :: use_gpu

   integer :: b=0   !! use b to detect if run on gpu

   !$omp metadirective &
   !$omp&            when(user={condition(use_gpu)}:           &
   !$omp&                 target teams distribute parallel for &
   !$omp&                 private(b) map(from:a(1:n)) )        &
   !$omp&            default(parallel do)
   do i = 1,n; a(i)=i; if(i==n) b=1; end do

   if(b==0) print *, "PASSED 1 of 3"  ! bc b is firstprivate for  gpu run 
end subroutine

subroutine bar (a, n, run_parallel, unbalanced)
   use omp_lib, only : omp_get_thread_num
   integer :: n, a(n)
   logical :: run_parallel, unbalanced

   integer :: b=0
   !$omp begin metadirective when(user={condition(run_parallel)}: parallel)

    if(omp_in_parallel() == 1 .and. omp_get_thread_num() == 0) &
       print *,"PASSED 2 of 3"

    !$omp metadirective &
    !$omp&  when(construct={parallel}, &
    !$omp&       user={condition(unbalanced)}:for schedule(guided) private(b)) &
    !$omp&  when(construct={parallel}        :for schedule(static)) 
    do i = 1,n; a(i)=i; if(i==n) b=1; end do

   !$omp end metadirective 

   if(b==0) print *, "PASSED 3 of 3"   !!if guided b=0, because b is private
end subroutine

program meta
   use omp_lib
   integer, parameter :: N=100
   integer :: p(N)
   integer :: env_stat
                      !! App normally sets these, dependent on input parameters
   logical ::  use_gpu=.true., run_parallel=.true., unbalanced=.true.

                      !! Testing: set Env Var MK_FAIL to anything to fail tests
   call get_environment_variable('MK_FAIL',status=env_stat)
   if(env_stat /= 1) then                ! status =1 when not set! 
      use_gpu=.false.; run_parallel=.false.; unbalanced=.false.
   endif


   call foo(p, N, use_gpu)
   call bar(p, N, run_parallel,unbalanced)

end program
