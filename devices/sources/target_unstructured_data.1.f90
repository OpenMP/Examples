! @@name:	target_unstructured_data.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.5
module example
  real(8), allocatable :: A(:)

  contains
    subroutine initialize(N)
      integer :: N

      allocate(A(N))
      !$omp target enter data map(alloc:A)

    end subroutine initialize

    subroutine finalize()

      !$omp target exit data map(delete:A)
      deallocate(A)

    end subroutine finalize
end module example
