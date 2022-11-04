! @@name:	allocators.4
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_5.1
subroutine test_allocate
   use omp_lib

   interface
     subroutine my_gpu_vxv(va,vb,n)
     !$omp declare target
     integer :: n
     double precision  :: va(n), vb(n)
     end subroutine
   end interface

   integer,parameter :: Nhb=1024*1024,   & !! high bandwith
                        Nbg=1024*1024*64,& !! big memory, default
                        Nll=1024*1024      !! low latency memory

   double precision  ::  v1(Nhb), v2(Nhb)
   double precision  ::  v3(Nbg), v4(Nbg)
   double precision  ::  v5(Nll), v6(Nll)

 !*** CASE 1: USING ALLOCATE DIRECTIVE ***!
   !$omp allocate(v1,v2) allocator(omp_high_bw_mem_alloc)
   !$omp allocate(v3,v4) allocator(omp_default_mem_alloc)  

   call my_init(v1,v2,Nhb, v3,v4,Nbg, v5,v6,Nll)

   call lib_saxpy(v1,v2,5.0,Nhb)

   !$omp target map(to: v3, v4) map(from:v3)
      call my_gpu_vxv(v3,v4,Nbg)
   !$omp end target

 !*** CASE 2: USING ALLOCATE CLAUSE ***!
   !$omp task private(v5,v6) &
   !$omp&     allocate(allocator(omp_low_lat_mem_alloc): v5,v6)
      call my_gather(v5,v6,Nll)
   !$omp end task

end subroutine test_allocate
