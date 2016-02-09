! @@name:	target.4f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
subroutine vec_mult(N)
   implicit none
   integer           :: i, N
   real, allocatable :: p(:), v1(:), v2(:)
   allocate( p(N), v1(N), v2(N) )
   !$omp parallel num_threads(2)
      !$omp single
         !$omp task depend(out:v1)
         call init(v1, N)
         !$omp end task
         !$omp task depend(out:v2)
         call init(v2, N)
         !$omp end task
         !$omp target nowait depend(in:v1,v2) depend(out:p) map(from: p) map(to:v1,v2)
         !$omp parallel do
         do i=1,N
            p(i) = v1(i) * v2(i)
         end do
         !$omp end target
         !$omp task depend(in:p)
         call output(p, N)
         !$omp end task
     !$omp end single
   !$omp end parallel
   deallocate( p, v1, v2 )
end subroutine
