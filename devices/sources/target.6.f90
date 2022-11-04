! @@name:	target.6
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.5
module params
integer,parameter :: THRESHOLD1=1000000, THRESHHOLD2=1000
end module

subroutine vec_mult(p, v1, v2, N)
   use params
   real    ::  p(N), v1(N), v2(N)
   integer ::  i

   call init(v1, v2, N)

   !$omp target parallel do  &
   !$omp&   if(target: N>THRESHHOLD1) if(parallel: N>THRESHOLD2) &
   !$omp&   map(to: v1, v2 ) map(from: p)
      do i=1,N
	 p(i) = v1(i) * v2(i)
      end do
   !$omp end target parallel do

   call output(p, N)
end subroutine
