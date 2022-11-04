! @@name:	collapse.2
! @@type:	F-fixed
! @@operation:	run
! @@expect:	success
! @@version:	omp_3.0
      program test
!$omp parallel
!$omp do private(j,k) collapse(2) lastprivate(jlast, klast)
      do k = 1,2
        do j = 1,3
          jlast=j
          klast=k
        enddo
      enddo
!$omp end do
!$omp single
      print *, klast, jlast !2, 3
!$omp end single
!$omp end parallel
      end program test
