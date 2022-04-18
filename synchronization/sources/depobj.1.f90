! @@name:       depobj.1
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success
! @@version: omp_5.2

program main
    use omp_lib
    implicit none
    
    integer,parameter        :: N=100
    real                     :: a(N),b(N)
    integer(omp_depend_kind) :: obj

    call init(a, N)

    !$omp depobj(obj) depend(inout: a)

    call driver(.true.,  a,b,N, obj)  !! updating occurs

    !$omp depobj(obj) update(in)

    call driver(.false., a,b,N, obj)  !! no updating

    !$omp depobj(obj) destroy(obj)    !! obj is set to uninitialized
                                      !! state, resources are freed

end program

subroutine driver(update, a, b, n, obj)
   use omp_lib
   implicit none
   logical :: update
   real    :: a(n), b(n)
   integer :: n
   integer(omp_depend_kind) :: obj

   !$omp parallel num_threads(2)

     !$omp single

       !$omp task depend(depobj: obj)       !! Task 1, uses depend object
         call update_copy(update, a,b,n)
              !! update a or not, always copy a to b
       !$omp end task

       !$omp task depend(in: a)             !! Task 2, only read a
         call checkpoint(a,n)
       !$omp end task

     !$omp end single

   !$omp end parallel

end subroutine

subroutine update_copy(update, a, b, n)
   implicit none
   logical :: update
   real    :: a(n), b(n)
   integer :: n
  
   if (update) a = a + 1.0

   b = a

end subroutine

subroutine checkpoint( a, n)
   implicit none
   integer :: n
   real    :: a(n) 
   integer :: i

   write(*,'( *(f5.0) )') (a(i), i=1,n)
end subroutine

subroutine init(a,n)
   implicit none
   integer :: n
   real    :: a(n) 
   integer :: i

   a=[ (i, i=1,n) ] 
end subroutine
