! @@name:	reduction.7
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	pre_omp_3.0
program array_red

  integer,parameter :: n=100
  integer           :: j 
  real              :: a(n), b(n,n)

  call init(n,b)

  a(:) = 0.0e0

  !$omp parallel do reduction(+:a)
  do j = 1, n
     a(:) = a(:) + b(:,j)
  end do

  print*, " a(1) a(n): ", a(1), a(n)

end program
