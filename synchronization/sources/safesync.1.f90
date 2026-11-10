! @@name:	safesync.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0

program safesync_1
   use omp_lib
   integer, parameter :: NT = 16, N = 100
   integer :: a(N), b(N)
   integer :: count1 = 0, count2 = 0
   integer :: i

   do i = 1, N
      a(i) = 0
      b(i) = i
   end do

   ! Rather than explicitly using the safesync clause below for a non-host
   ! device, the following requires directive can make the "safesync"
   ! behavior the default for parallel constructs encountered on a non-host
   ! device.
   ! !$omp requires device_safesync

   !$omp target thread_limit(NT) map(to:count1,count2) map(a) map(to:b)
   !$omp metadirective &
   !$omp& when(device={kind(nohost)}: parallel num_threads(NT) safesync) &
   !$omp& otherwise(parallel num_threads(NT))
   block
      integer :: t, u
      !$omp atomic capture
         t = count1
         count1 = count1 + 1
      !$omp end atomic
      do
         !$omp atomic read acquire
         u = count2
         if (u >= t) exit
      end do

      do i = 1, N
         a(i) = a(i) + b(i)
      end do

      !$omp atomic release
      count2 = count2 + 1
   end block
   !$omp end target

   do i = 1, N
      if (a(i) /= NT*b(i)) then
         print *, "    a(", i, ") = ", a(i), ", b(", i, &
                  ") = ", b(i)
         error stop
      end if
   end do

end program

