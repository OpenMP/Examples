! @@name:       affinity.6
! @@type:       F-free
! @@compilable: yes
! @@linkable:   no
! @@expect:     success
! @@version:	omp_5.0
subroutine task_affinity(A, N)

  external alloc_init_B
  external compute_on_B
  double precision, allocatable :: B(:)

   !$omp task depend(out:B) shared(B) affinity(A)
     call alloc_init_B(B,A)
   !$omp end task 

   !$omp task depend(in:B) shared(B) affinity(A)
     call compute_on_B(B)
   !$omp end task 

   !$omp taskwait

end subroutine

