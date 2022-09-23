! @@name:       target_mapper.2
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.0
module complex_mats

   integer, parameter :: N=100    !N must be even
   type dzmat_t
     double precision ::  r_m(N,N), i_m(N,N)
   end type

   !$omp  declare mapper( left_id: dzmat_t :: v) map( v%r_m(N,  1:N/2), &
   !$omp&                                             v%i_m(N,  1:N/2))

   !$omp  declare mapper(right_id: dzmat_t :: v) map( v%r_m(N,N/2+1:N), &
   !$omp&                                             v%i_m(N,N/2+1:N))

end module


program main
  use  complex_mats
  type(dzmat_t) :: a,b,c
  external dzmat_init, host_add  !initialization and matrix add: a=b+c

  integer :: is,ie


  is=1; ie=N/2            !left N/2 columns on device 0
  !$omp target map(mapper( left_id), tofrom: a,b) device(0) &
  !$omp&       firstprivate(is,ie) nowait
    call dzmat_init(a,is,ie)
    call dzmat_init(b,is,ie)
  !$omp end target

  is=N/2+1; ie=N         !right N/2 columns on device 1
  !$omp target map(mapper(right_id), tofrom: a,b) device(1) &
  !$omp&       firstprivate(is,ie) nowait
    call dzmat_init(a,is,ie)
    call dzmat_init(b,is,ie)
  !$omp end target

  !$omp taskwait

  call host_add(a,b,c)

end program main
