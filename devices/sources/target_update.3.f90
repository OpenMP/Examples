! @@name:       target_update.3
! @@type:       F-free
! @@operation:  run
! @@expect:     success
! @@version:    omp_5.1
      module my_struct
       type T
        integer :: x,y,z
       end type
      end module

      program main
       use my_struct
       integer, parameter :: N=100
       integer  :: a,b,c

      !$omp declare mapper(custom: T :: v) &
      !$omp& map(to:v%x) map(from:v%y) map(alloc: v%z)

       type(T) :: s

       s%x = 5
       s%y = 5
       s%z = 5

       !$omp target data map(mapper(custom),tofrom: s)

       s%x = s%x + 5
       s%y = s%y + 5
       s%z = s%z + 5
 
       !$omp target update to(mapper(custom) : s)

       !$omp target map(from: a,b,c)
         a = s%x
         b = s%y
         c = s%z
        
         s%y = 5
         print*,"s%x:", s%x, " s%y:", s%y
              !! s%x:10, s%y:5 (value of s%z is undefined)
       !$omp end target
  
       !$omp target update from(mapper(custom) : s)
       print*, "s%y:", s%y !! s%y:5
       print*, "a:", a     !! a:10 (values of b and c are undefined)

       !$omp end target data
       
       print*, "s%x:", s%x, " s%y:", s%y, " s%z:", s%z
            !! s%x:10, s%y:5, s%z:10
      end program
