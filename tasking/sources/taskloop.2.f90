! @@name:   taskloop.2
! @@type:   F-free
! @@compilable: yes
! @@linkable:   no
! @@expect: success
! @@version: omp_4.5
subroutine parallel_work
    implicit none
    integer :: x1, x2
    integer :: i
    integer, parameter :: T = 16
    integer, parameter :: N = 1024

    x1 = 0
    x2 = 0
    !$omp parallel shared(x1,x2) num_threads(T)
    !$omp taskloop
    do i = 1,N
        !$omp atomic
        x1 = x1 + 1     ! executed T*N times
        !$omp end atomic
    end do
    !$omp end taskloop

    !$omp single
    !$omp taskloop
    do i = 1,N
        !$omp atomic
        x2 = x2 + 1     ! executed N times
        !$omp end atomic
    end do
    !$omp end taskloop
    !$omp end single
    !$omp end parallel

    write (*,'(A,I0,A,I0)') 'x1 = ', x1, ', x2 = ',x2
end subroutine
