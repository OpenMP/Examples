! @@name:	allocators.5
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

    integer( omp_allocator_handle_kind ) :: cgroup_alloc
    type(omp_alloctrait),parameter       :: cgroup_traits(1)= &
                         [omp_alloctrait(omp_atk_access,omp_atv_cgroup)]

    do i=1,N; xbuf(i)=0; end do

!*** CASE 1: USING ALLOCATE DIRECTIVE ***!

    !! uses predefined allocator omp_cgroup_mem_alloc

    !$omp target uses_allocators(omp_cgroup_mem_alloc)
    !$omp teams  reduction(+:xbuf) thread_limit(N) &
    !$omp&       allocate(omp_cgroup_mem_alloc:xbuf) num_teams(4)
  
        !$omp parallel do
        do i = 1,N 
           xbuf(i) = xbuf(i) + calc(i, omp_get_team_num())
        enddo

    !$omp end teams
    !$omp end target

    sum = 0
    !$omp parallel do reduction(+:sum)
    do i = 1,N 
        sum = sum + xbuf(i)
    enddo
    if(sum == 3*(N+1)*N) print*, "PASSED 1 of 3"

!*** CASE 2: ***!

    do i=1,N; xbuf(i)=0; end do

    cgroup_alloc = omp_null_allocator

    !! uses custom allocator with specified traits
    !$omp  target uses_allocators(traits(cgroup_traits): cgroup_alloc)
    !$omp  teams  reduction(+:xbuf) thread_limit(N) &
    !$omp&        allocate(cgroup_alloc:xbuf) num_teams(4)

        !$omp parallel do
        do i = 1,N
           xbuf(i) = xbuf(i) + calc(i,omp_get_team_num())
        enddo

    !$omp end teams
    !$omp end target

    sum = 0 
    !$omp parallel do reduction(+:sum)
    do i = 1,N 
        sum = sum + xbuf(i)
    enddo
    if(sum == 3*(N+1)*N) print*, "PASSED 2 of 3"

!*** CASE 3: ***!

    do i=1,N; xbuf(i)=0; end do

    cgroup_alloc = omp_init_allocator(omp_default_mem_space, 1, &
                                      cgroup_traits)

    !! WARNING: uses custom allocator but with DEFAULT traits
    !$omp  target uses_allocators(cgroup_alloc)
    !$omp  teams  reduction(+:xbuf) thread_limit(N) &
    !$omp&        allocate(cgroup_alloc:xbuf) num_teams(4)

        !$omp parallel do
        do i = 1,N
            xbuf(i) = xbuf(i) + calc(i,omp_get_team_num())
        enddo

    !$omp end teams
    !$omp end target
    
    call omp_destroy_allocator(cgroup_alloc)

    sum = 0
    !$omp parallel do reduction(+:sum)
    do i = 1,N
        sum = sum + xbuf(i)
    enddo
    if(sum == 3*(N+1)*N) print*, "PASSED 3 of 3"

end program main
