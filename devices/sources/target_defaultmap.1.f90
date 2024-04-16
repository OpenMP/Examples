! @@name:	target_defaultmap.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
program defaultmap
  integer, parameter :: N=2

  type DDT_sA
    integer  :: s
    integer  :: A(N)
  end type

  integer             :: s,s1,s2,s3 !! SCALAR: variable (integer)
  integer,target      :: A(N)       !! AGGREGATE: Array
  type(DDT_sA)        :: D          !! AGGREGATE: Derived Data Type (D)
  integer,allocatable :: H(:)       !! ALLOCATABLE: Heap allocated array
  integer,pointer     :: ptrA(:)    !! POINTER: points to array

  ! Assign values to scalar, array, allocatable, and pointers

    s=2
    s1=0;   s2=0;     s3=0
    D%s=0;  D%A(1)=0; D%A(2)=0
    A(1)=0; A(2)=0

    allocate( H(2) )
    H(1)=0; H(2)=0

!! Target Region 1
                   !! Using defaultmap to set scalars, aggregates & 
                   !! pointers and allocatables to normal defaults.
    !$omp target                                        &
    !$omp&       defaultmap( firstprivate: scalar)      &
    !$omp&       defaultmap( tofrom:       aggregate)   &
    !$omp&       defaultmap( tofrom:       allocatable) &
    !$omp&       defaultmap( default:      pointer) 

        s = 3                     !! SCALAR firstprivate, val not returned
 
        A(1) = 3                  !! AGGREGATE array, default map tofrom
        A(2) = 3

        D%s = 2                   !! AGGR. Derived Type, default map tofrom
        D%A(1) = 2;  D%A(2) = 2

        H(1) = 2;    H(2) = 2     !! ALLOCATABLE, default map tofrom

        ptrA=>A                   !! POINTER is private
        ptrA(1) = 2; ptrA(2) = 2

    !$omp end target

    if(s==2 .and. A(1)==2 .and. D%s==2 .and. D%A(1)==2 .and. H(1) == 2) &
       print*," PASSED 1 of 5"

!! Target Region 2
                   !! no implicit mapping allowed
    !$omp target defaultmap(none) map(tofrom: s, A, D)

        s=s+5                 !! All variables must be explicitly mapped
        A(1)=A(1)+5;       A(2)=A(2)+5
        D%s=D%s+5 
        D%A(1)=D%A(1)+5; D%A(2)=D%A(2)+5 

    !$omp end target
    if(s==7 .and. A(1)==7 .and. D%s==7 .and. D%A(1)==7) &
        print*," PASSED 2 of 5"

!! Target Region 3
            !! defaultmap & explicit data-sharing clause
            !! with variables in same category
    s1=1; s2=1; s3=1
    !$omp target defaultmap(tofrom: scalar) firstprivate(s1,s2)

        s1 = s1+5          !! firstprivate (s1 value not returned to host)
        s2 = s2+5          !! firstprivate (s2 value not returned to host)
        s3 = s3 +s1 + s2   !! mapped as tofrom

    !$omp end target
    if(s1==1 .and. s2==1 .and. s3==13) print*," PASSED 3 of 5"

!! Target Region 4
    A(1)=0;   A(2)=0
    D%A(1)=0; D%A(2)=0
    H(1)=0;   H(2)=0
              !! non-allocated arrays & derived types are in AGGREGATE cat
              !! Allocatable arrays are in ALLOCATABLE category
              !! Scalars are explicitly mapped from
    !$omp target defaultmap(firstprivate: aggregate )  &
    !$omp&       defaultmap(firstprivate: allocatable) &
    !$omp&       map(from: s1, s2)

        A(1)=A(1)+1; D%A(1)=D%A(1)+1; H(1)=H(1)+1 !! changes to A, D%A, H
        A(2)=A(2)+1; D%A(2)=D%A(2)+1; H(2)=H(2)+1 !! not returned to host
        s1 = A(1)+D%A(1)+H(1)                     !! s1 returned to host
        s2 = A(2)+D%A(2)+H(1)                     !! s2 returned to host

    !$omp end target
    if(A(1)==0 .and. D%A(1)==0 .and. H(1)==0 .and. s1==3) &
       print*," PASSED 4 of 5"

!! Target Region 5
            !! defaultmap & explicit data-sharing clause
            !! with variables in same category
    s1=1; s2=1; s3=1
    !$omp target defaultmap(to: all) map(from: s3)

        s1 = s1+5        !! mapped as to
        s2 = s2+5        !! mapped as to
        s3 = s1 + s2     !! mapped as from

    !$omp end target
    if(s1==1 .and. s2==1 .and. s3==12) print*," PASSED 5 of 5"

    deallocate(H)

end program
