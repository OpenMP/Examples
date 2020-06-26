! @@name:       target_mapper.3.f90
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:    omp_5.0

module my_structures
  type myvec_t
    integer                   :: len
    double precision, pointer :: data(:)
  end type
  !$omp  declare mapper(myvec_t :: v) &
  !$omp&         map(v)

  type mypoints_t
     type(myvec_t)            :: scratch
     type(myvec_t), pointer   :: x(:)
     double precision         :: hostonly_data(500000)
  end  type
 !$omp  declare mapper(mypoints_t :: v)  &
 !$omp&         map(v%x, v%x(1)) map(alloc:v%scratch)

end module


program main
  use my_structures
  external  init_mypts_array, eval_mypts_array

  type(mypoints_t) :: P

   call init_mypts_array(P)

   !$omp target map(P)
   call eval_mypts_array(P)

end program

