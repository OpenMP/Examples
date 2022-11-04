! @@name:	associate.1
! @@type:	F-fixed
! @@operation:	compile
! @@expect:	ct-error
! @@version:	omp_4.0
      program example_broken
      real :: a, c
      associate (b => a)
!$omp parallel private(b, c)        ! invalid to privatize b
      c = 2.0*b
!$omp end parallel
      end associate
      end program
