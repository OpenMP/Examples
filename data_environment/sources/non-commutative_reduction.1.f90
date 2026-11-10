! @@name:       non-comm-reduce.4
! @@type:       F-free
! @@operation:  run 
! @@expect:     success
! @@version:    omp_6.1
module matrix_defs
  implicit none
  integer,parameter :: N=2     !size of square matrix
  integer,parameter :: Nmats=6 !number of matrices

  type :: Matrix
    integer, dimension(N,N) :: mat
  end type

  interface operator(*)
    module procedure mxm
  end interface

contains
  function mxm(A, B) result(C)
    type(Matrix), intent(in) :: A, B
    type(Matrix)             :: C
    C%mat = matmul(A%mat, B%mat)
  end function

  function m_unity() result(lmat)
    type(Matrix)         :: lmat
    integer :: i
    lmat%mat=0
    do i = 1,N
      lmat%mat(i,i)=1
    enddo
  end function

end module

program matrix_chain_multiply
  use matrix_defs
  implicit none
  integer   :: i, k
  type(Matrix) :: A(Nmats)
  type(Matrix) :: parallel_prod, serial_prod

  !$omp  declare_reduction( *: Matrix ) &
  !$omp& combiner( omp_out = omp_out*omp_in ) &
  !$omp& initializer( omp_priv = m_unity() )

  ! Initialize with sequence of numbers
  do  i = 1,Nmats
    A(i)%mat=reshape( [(k, k=1,N*N)], [N, N] )
  enddo

  ! Product:  A(1)*A(2)*...*A(N)
  parallel_prod=m_unity()
  !$omp  parallel do &
  !$omp& reduction(noncommutative, *: parallel_prod) num_threads(Nmats/2)
  do i = 1,Nmats
    parallel_prod = parallel_prod * A(i)
  end do

  serial_prod=m_unity()
  do i = 1,Nmats
    serial_prod = serial_prod * A(i)
  end do

  if ( all(parallel_prod%mat == serial_prod%mat) ) then
    print*, "passed"
  else
    print*, "failed"
  endif

end program
