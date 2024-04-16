! @@name:       usm_scalar_ptr_ref_ax.1
! @@type:       F-free
! @@operation:  compile
! @@expect:     success
! @@version:    omp_5.2
program main
   !$omp requires unified_shared_memory
   
   logical          :: pass=.TRUE.

   integer          :: x
   integer, target  :: y
   integer, pointer :: ptr 

   x = 0 
   ! Case 1 : x is firstprivate
   !$omp target
      x = x + 1  
   !$omp end target
   if(x /= 0 ) pass = .FALSE.

   x = 0 
   ASSOCIATE( ax => x)

   ! Case 2 : 
   !$omp target    
      ax = ax + 1 
   !$omp end target    
   if(x /= 1 ) pass = .FALSE.

   end ASSOCIATE

   y = 0
   ptr => y

   ! Case 3a : ptr is mapped
   !$omp target
      ptr = ptr + 1
   !$omp end target
   if(y /= 1 ) pass = .FALSE.

   y = 0

   ! Case 3b : y is mapped
   !$omp target
      y = y + 1
   !$omp end target
   if(y /= 1 ) pass = .FALSE.
  

  if(pass) then
    print*, "PASSED"
  else 
    print*, "FAILED"; stop 1
  endif

end program
