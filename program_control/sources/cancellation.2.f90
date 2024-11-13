! @@name:	cancellation.2
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
module parallel_search
  type binary_tree
    integer :: value
    type(binary_tree), pointer :: right
    type(binary_tree), pointer :: left
  end type

contains
  recursive function search_tree(tree, value, level) result(found)
    type(binary_tree), intent(in), pointer :: tree
    integer, intent(in) :: value, level
    type(binary_tree), pointer :: found
    type(binary_tree), pointer :: found_left, found_right

    found => NULL()
    if (associated(tree)) then
      if (tree%value .eq. value) then
        found => tree
      else
!$omp task shared(found) if(level<10)
        found_left => search_tree(tree%left, value, level+1)
        if (associated(found_left)) then
!$omp atomic write
          found => found_left
!$omp end atomic

!$omp cancel taskgroup
        endif
!$omp end task

!$omp task shared(found) if(level<10)
        found_right => search_tree(tree%right, value, level+1)
        if (associated(found_right)) then
!$omp atomic write
          found => found_right
!$omp end atomic

!$omp cancel taskgroup
        endif
!$omp end task

!$omp taskwait
      endif
    endif
  end function

  subroutine search_tree_parallel(tree, value, found)
    type(binary_tree), intent(in), pointer :: tree
    integer, intent(in) :: value
    type(binary_tree), pointer :: found

    found => NULL()
!$omp parallel shared(found, tree, value)
!$omp masked
!$omp taskgroup
    found => search_tree(tree, value, 0)
!$omp end taskgroup
!$omp end masked
!$omp end parallel
  end subroutine

end module parallel_search
