! @@name:	allocators.6
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
module functions
contains
   function calc(i,j)  result(ii)
     implicit none
     integer :: i,j,ii
     !$omp declare target(calc)

     ii = i*j
   end function
end module

program main

    use omp_lib
    use functions
    implicit none
    integer, parameter :: N=256
    integer :: sum, i
    integer :: xbuf(N)

    !$omp requires dynamic_allocators

    integer( omp_allocator_handle_kind ) :: cgroup_alloc
    type(omp_alloctrait),parameter       :: cgroup_traits(1)= &
                         [omp_alloctrait(omp_atk_access,omp_atv_cgroup)]
                    
!*** CASE 1: ***!

    do i=1,N; xbuf(i)=0; end do

    !! uses predefined allocator, no need to declare it in uses_allocators
    !$omp  target teams reduction(+:xbuf) thread_limit(N) &
    !$omp&              allocate(omp_cgroup_mem_alloc:xbuf) num_teams(4)

        !$omp parallel do
        do i = 1,N
           xbuf(i) = xbuf(i) + calc(i,omp_get_team_num())
        enddo

    !$omp end target teams

    sum = 0
    !$omp parallel do reduction(+:sum)
    do i = 1,N
        sum = sum + xbuf(i)
    enddo
    if(sum == 3*(N+1)*N) print*, "PASSED 1 of 2"

!*** CASE 2: ***!

    do i=1,N; xbuf(i)=0; end do

    cgroup_alloc = omp_init_allocator(omp_default_mem_space, 1, &
                                      cgroup_traits)

    !! WARNING: cgroup_alloc is in undefined state on target device!
    !$omp  target teams reduction(+:xbuf) thread_limit(N) &
    !$omp&              allocate(cgroup_alloc:xbuf) num_teams(4)

        !$omp parallel do
        do i = 1,N
           xbuf(i) = xbuf(i) + calc(i,omp_get_team_num())
        enddo

    !$omp end target teams

    call omp_destroy_allocator(cgroup_alloc)

    sum = 0
    !$omp parallel do reduction(+:sum)
    do i = 1,N
        sum = sum + xbuf(i)
    enddo
    if(sum == 3*(N+1)*N) print*, "PASSED 2 of 2"

end program main
