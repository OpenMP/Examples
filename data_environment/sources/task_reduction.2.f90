! @@name:	task_reduction.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
program task_modifier

   integer :: N=100, M=10
   integer :: i, x

! USE CASE 1  explicit-task reduction + parallel reduction clause
   x=0 
   !$omp parallel num_threads(M) reduction(task,+:x) 

     x=x+1                   !! implicit task reduction statement

     !$omp single
       do i = 1,N
         !$omp task in_reduction(+:x)
           x=x+1
         !$omp end task
       end do
     !$omp end single
   
   !$omp end parallel
   write(*,'("x=",I0," =M+N")') x   ! x= 110 =M+N


! USE CASE 2  task reduction +  worksharing reduction clause
   x=0
   !$omp parallel do num_threads(M) reduction(task,+:x)
     do i = 1,N
  
        x=x+1
  
        if( mod(i,2) == 0) then
           !$omp task in_reduction(+:x)
             x=x-1
           !$omp end task
        endif
  
     end do
   write(*,'("x=",I0,"  =N-N/2")') x   ! x= 50 =N-N/2

end program
