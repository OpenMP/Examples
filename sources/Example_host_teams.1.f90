! @@name: host_teams.2.f90
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success
! @@version: omp_5.0

program main
   use omp_lib
   integer           :: nteams_required=2, max_thrds, tm_id
   integer,parameter ::  N=1000
   real              :: sp_x(N), sp_y(N), sp_a=0.0001e0
   double precision  :: dp_x(N), dp_y(N), dp_a=0.0001d0

   max_thrds  = omp_get_num_procs()/nteams_required
 
   !! Create 2 teams, each team works in a different precision
   !$omp teams num_teams(nteams_required) thread_limit(max_thrds) private(tm_id)

      tm_id = omp_get_team_num()

      if( omp_get_num_teams() /= 2 ) then   !! if only getting 1, quit
         stop "error: Insufficient teams on host, 2 required."
      endif

      if(tm_id == 0) then    !! Do Single Precision Work (SAXPY) with this team

         !$omp parallel 
            !$omp do         !! init
            do i = 1,N
               sp_x(i) = i*0.0001e0
               sp_y(i) = i
            end do

            !$omp do simd simdlen(8)
            do i = 1,N
               sp_x(i) = sp_a*sp_x(i) + sp_y(i)
            end do
         !$omp end parallel 

      endif

      if(tm_id == 1) then    !! Do Double Precision Work (DAXPY) with this team

         !$omp parallel 
            !$omp do         !! init
            do i = 1,N
               dp_x(i) = i*0.0001d0
               dp_y(i) = i
            end do

            !$omp do simd simdlen(4)
            do i = 1,N
               dp_x(i) = dp_a*dp_x(i) + dp_y(i)
            end do
         !$omp end parallel 

      endif
   !$omp end teams

   write(*,'( "i=",i4," sp|dp= ", e15.7, d25.16  )') N, sp_x(N), dp_x(N)
   write(*,'( "i=",i4," sp|dp= ", e15.7, d25.16  )') N/2, sp_x(N/2), dp_x(N/2)
            !! i=1000 sp|dp=   0.1000000E+04   0.1000000010000000D+04
            !! i= 500 sp|dp=   0.5000000E+03   0.5000000050000000D+03
end program
