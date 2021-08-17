! @@name:       unroll.4.f
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.1
subroutine unroll_partial_remainder(n, A)
   implicit none
   integer :: n, i
   integer :: A(*)

    !$omp parallel do
    !$omp unroll partial(4)
    do i = 1, n
       A(i) = i
    end do

end subroutine

subroutine unroll_partial_remainder_option1(n, A)
   implicit none
   integer :: n, i_iv
   integer :: A(*)

   !$omp parallel do
   do i_iv = 0,(n+3)/4 -1
                               A(i_iv * 4 + 1) = i_iv * 4 + 1
        if (i_iv * 4 + 2 <= n) A(i_iv * 4 + 2) = i_iv * 4 + 2
        if (i_iv * 4 + 3 <= n) A(i_iv * 4 + 3) = i_iv * 4 + 3
        if (i_iv * 4 + 4 <= n) A(i_iv * 4 + 4) = i_iv * 4 + 4
   end do

end subroutine

subroutine unroll_partial_remainder_option2(n, A)
   implicit none
   integer :: n, i_iv, i_rem
   integer :: A(*)

    !$omp parallel do
    do i_iv = 0, (n+3)/4 -1
        if (i_iv < n/4) then
            A(i_iv * 4 + 1) = i_iv * 4 + 1
            A(i_iv * 4 + 2) = i_iv * 4 + 2
            A(i_iv * 4 + 3) = i_iv * 4 + 3
            A(i_iv * 4 + 4) = i_iv * 4 + 4
        else
           !! remainder loop
           do i_rem = i_iv*4 +1, n
              A(i_rem) = i_rem
           end do
        end if
    end do

end subroutine

subroutine unroll_partial_remainder_option3(n, A)
   implicit none
   integer :: n, i_iv, i_rem
   integer :: A(*)

    !$omp parallel do
    do i_iv = 0, (n/4) -1

       A(i_iv * 4 + 1) = i_iv * 4 + 1
       A(i_iv * 4 + 2) = i_iv * 4 + 2
       A(i_iv * 4 + 3) = i_iv * 4 + 3
       A(i_iv * 4 + 4) = i_iv * 4 + 4
    end do

    !! remainder loop
    !$omp parallel do
    do i_rem = (n/4)*4 +1, n
       A(i_rem) = i_rem
    end do

end subroutine

program main
implicit none
integer, parameter :: NT=12

integer :: i
logical :: error=.false.
integer   :: A(NT), C(NT)=[ (i, i=1,NT) ]

    A(1:NT)=0
    call unroll_partial_remainder(NT, A)
    if( .not. all(A(1:NT) == C(1:NT)) ) error = .true.

    A(1:NT)=0
    call unroll_partial_remainder_option1(NT, A)
    if( .not. all(A(1:NT) == C(1:NT)) ) error = .true.

    A(1:NT)=0
    call unroll_partial_remainder_option2(NT, A)
    if( .not. all(A(1:NT) == C(1:NT)) ) error = .true.

    A(1:NT)=0
    call unroll_partial_remainder_option3(NT, A)
    if( .not. all(A(1:NT) == C(1:NT)) ) error = .true.

    if(.not. error) print*, "OUT: Passed."
    if(      error) print*, "OUT: Failed"
end program
