! @@name:	allocators.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
program main
 use omp_lib

 integer, parameter :: N=1000
 real, allocatable  :: x(:),y(:)
 real               :: s = 2.0e0
 integer            :: i 

 integer(omp_memspace_handle_kind ) :: xy_memspace = omp_default_mem_space
 type(   omp_alloctrait           ) :: xy_traits(1) = &
                                    [omp_alloctrait(omp_atk_alignment,64)]
 integer(omp_allocator_handle_kind) :: xy_alloc

   xy_alloc   =    omp_init_allocator(   xy_memspace, 1, xy_traits)

   !$omp allocators allocate(allocator(xy_alloc): x, y) 
   allocate(x(N),y(N))
                         !! loc is non-standard, but found everywhere
                         !! remove these lines if not available
   if(modulo(loc(x),64) /= 0 .and. modulo(loc(y),64) /=0 ) then
      print*,"ERROR: x|y not 64-byte aligned"; stop
   endif

   !$omp parallel

      !$omp do simd simdlen(16) aligned(x,y: 64) !! 64B aligned
      do i=1,N  !! initialize
        x(i)=i 
        y(i)=i
      end do

      !$omp do simd simdlen(16) aligned(x,y: 64) !! 64B aligned
      do i = 1,N
         y(i) = s*x(i) + y(i)
      end do

   !$omp end parallel

   write(*,'("y(1),y(N):",2f6.0)') y(1),y(N) !!output: y... 3. 3000.

   deallocate(x,y)
   call omp_destroy_allocator(xy_alloc)

end program
