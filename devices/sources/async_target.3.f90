! @@name:	async_target.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.1
program concurrent_async
   use omp_lib
   integer,parameter :: n=1000000  !!n must be even
   integer           :: i, chunk=1000
   real              :: v1(n),v2(n),vxv(n)

   call init(n, v1,v2)

   !$omp parallel

      !$omp masked
      !$omp target teams distribute parallel do nowait &
      !$omp&                    map(to: v1(1:n/2))   &
      !$omp&                    map(to: v2(1:n/2))   & 
      !$omp&                    map(from: vxv(1:n/2)) 
      do i = 1,n/2;    vxv(i) = v1(i)*v2(i); end do
      !$omp end masked

      !$omp do schedule(dynamic,chunk)
      do i = n/2+1,n;  vxv(i) = v1(i)*v2(i); end do

   !$omp end parallel

   print*, " vxv(1) vxv(n) :", vxv(1), vxv(n)

end program
