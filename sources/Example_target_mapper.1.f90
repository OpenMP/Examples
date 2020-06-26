! @@name:       target_mapper.1.f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.0

module my_structures
  type myvec_t
    integer                     :: len
    double precision, pointer   :: data(:)
  end type
end module

program main
  use my_structures
  integer, parameter :: N=100

  !$omp  declare mapper(myvec_t :: v) &
  !$omp&         map(v, v%data(1:v%len))

  type(myvec_t) :: s

  allocate(s%data(N))
  s%data(1:N) = 0.0d0
  s%len = N

  !$omp target
  call init(s)
  !$omp end target

  print*,"s%data(",N,")=",s%data(N)  !! s%data( 100 )=100.000000000000
end program

subroutine init(s)
  use my_structures
  type(myvec_t) :: s

  s%data = [ (i, i=1,s%len) ]
end subroutine
