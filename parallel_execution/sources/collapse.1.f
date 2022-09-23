! @@name:	collapse.1
! @@type:	F-fixed
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_3.0

      subroutine sub(a)

      real a(*)
      integer kl, ku, ks, jl, ju, js, il, iu, is
      common /csub/ kl, ku, ks, jl, ju, js, il, iu, is
      integer i, j, k

!$omp do collapse(2) private(i,j,k)
       do k = kl, ku, ks
         do j = jl, ju, js
           do i = il, iu, is
             call bar(a,i,j,k)
          enddo
        enddo
      enddo
!$omp end do

      end subroutine
