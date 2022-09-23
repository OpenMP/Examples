! @@name:	reduction.7
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
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
