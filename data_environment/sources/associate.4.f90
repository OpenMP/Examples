! @@name:	associate.4
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_5.1
program main
  integer :: scalr, aray(3)
  scalr = -1 ; aray = -1

  associate(a_scalr=>scalr, a_aray=>aray)

 !$omp target            !! TARGET 1
    aray = [1,2,3]
  !$omp end target
  print *, a_aray, aray  !!  1 2 3   1 2 3 

  !$omp target           !! TARGET 2
    a_aray = [4,5,6]
  !$omp end target
  print *, a_aray, aray  !! 4 5 6   4 5 6 

!!!$omp target           !! TARGET 3
!!                       !! mapping, in this case implicit,
!!                       !! of aray AND a_aray NOT ALLOWED
!!    aray = [4,5,6] 
!!  a_aray = [1,2,3]
!!!$omp end target


  !$omp target              !! TARGET 4
    scalr = 1               !! scalr is firstprivate
  !$omp end target
  print *, a_scalr, scalr   !! -1  -1  

  !$omp target              !! TARGET 5
    a_scalr = 2             !! a_scalr implicitly mapped 
  !$omp end target
  print *, a_scalr, scalr   !!  2   2   

  !$omp target              !! TARGET 6
    scalr = 3               !!          scalr is firstprivate
    print *, a_scalr, scalr !!  2   3   
    a_scalr = 4             !!          a_scalr implicitly mapped
    print *, a_scalr, scalr !!  4   3   
  !$omp end target
  print *, a_scalr, scalr   !!  4   4   

!!!$omp target map(a_scalr,scalr)  !! TARGET 7
                                   !! mapping, in this case explicit,
                                   !! of scalr AND a_sclar NOT ALLOWED
!!    scalr = 5 
!!  a_scalr = 5 
!!!$omp end target

  end associate

end program
