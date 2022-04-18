! @@name:       task_reduction.1
! @@type:       F-free
! @@compilable: yes
! @@linkable:   yes
! @@expect:     success

module m
    type node_t
        integer :: val
        type(node_t), pointer :: next
    end type
end module m

function linked_list_sum(p) result(res)
    use m
    implicit none
    type(node_t), pointer :: p
    type(node_t), pointer :: aux
    integer :: res

    res = 0

    !$omp taskgroup task_reduction(+: res)
        aux => p
        do while (associated(aux))
            !$omp task in_reduction(+: res)
                res = res + aux%val
            !$omp end task
            aux => aux%next
        end do
    !$omp end taskgroup
end function linked_list_sum


program main
    use m
    implicit none
    type(node_t), pointer :: root, aux
    integer :: res, i
    integer, parameter :: N=10

    interface
        function linked_list_sum(p) result(res)
            use m
            implicit none
            type(node_t), pointer :: p
            integer :: res
        end function
    end interface
!                       Create the root node.
    allocate(root)
    root%val = 1
    aux => root

!                       Create N-1 more nodes.
    do i = 2,N
        allocate(aux%next)
        aux => aux%next
        aux%val = i
    end do

    aux%next => null()

    !$omp parallel
    !$omp single
        res = linked_list_sum(root)
        print *, "Calculated:", res, " Analytic:", (N*(N+1))/2
    !$omp end single
    !$omp end parallel

end program main

