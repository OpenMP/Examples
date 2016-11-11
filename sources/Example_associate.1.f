! @@name:	associate.1f
! @@type:	F-fixed
! @@compilable:	no
! @@linkable:	no
! @@expect:	failure
      program example
      real :: a, c
      associate (b => a)
!$omp parallel private(b, c)        ! invalid to privatize b
      c = 2.0*b
!$omp end parallel
      end associate
      end program
