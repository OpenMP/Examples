! @@name:       apply_nested_equivalent.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_6.0
subroutine step1(A)
   implicit none
   double precision :: A(0:99)
   integer          :: i,i1,i2

    do i1 = 0, 9
    !$omp unroll partial(2) apply(reverse)
    do i2 = 0, 9
       i = i1 * 10 + i2
       A(i) = A(i) + 1
    enddo; enddo
end subroutine

subroutine step2(A)
   implicit none
   double precision :: A(0:99)
   integer          :: i,i1,i2

    do i1 = 0, 9
    !$omp reverse
    do i2 = 0, 4
       i = i1 * 10 + i2 * 2
       A(i) = A(i) + 1
       i = i + 1
       A(i) = A(i) + 1
    enddo; enddo
end subroutine

subroutine step3(A)
   implicit none
   double precision :: A(0:99)
   integer          :: i,i1,i2

    do i1 = 0, 9
    do i2 = 4, 0, -1
       i = i1 * 10 + i2 * 2
       A(i) = A(i) + 1
       i = i + 1
       A(i) = A(i) + 1
    enddo; enddo
end subroutine
