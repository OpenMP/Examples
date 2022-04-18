! @@name:	associate.3
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_4.0
program example
  integer :: v
  v = 15
associate(u => v)
!$omp parallel private(v)
  v = -1
  print *, v               ! private v=-1
  print *, u               ! original v=15
!$omp end parallel
end associate
end program
