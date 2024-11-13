! @@name:       dispatch.1
! @@type:       F-free
! @@operation:  run 
! @@expect:     success
! @@version:    omp_6.0
module funcs
  logical :: foo_sub

contains
  subroutine foo_variant1()
    print*, "in foo_variant1"
  end subroutine

  subroutine foo_variant2()
    print*, "in foo_variant2"
  end subroutine

  subroutine foo()
    !$omp  declare variant(foo_variant1) &
    !$omp&         match(user={condition(foo_sub)}) 
    !$omp  declare variant(foo_variant2) &
    !$omp          match(construct={dispatch},user={condition(foo_sub)}) 
    print*, "in foo"
  end subroutine

end module funcs

program main
  use funcs

  !! Case 1
  foo_sub = .TRUE.
  call foo()         !! "in foo_variant1"

  !! Case 2
  foo_sub = .FALSE.
  call foo()         !! "in foo"

  !! Dispatch Cases

  !! Case 3
  foo_sub=.FALSE.
  !$omp dispatch
  call foo()         !! "in foo"
   
  !! Case 4
  foo_sub = .TRUE.
  !$omp dispatch
  call foo();        !! "in foo_variant2"
                     !! see discussion for OpenMP 5.1/5.2

  !! Case 5
  foo_sub = .TRUE.
  !$omp dispatch novariants(.true.)
  call foo();        !!  "in foo"

  !! Case 6
  foo_sub = .TRUE.
  !$omp dispatch nocontext(.true.)
  call foo();        !! "in foo_variant1"

end program 
