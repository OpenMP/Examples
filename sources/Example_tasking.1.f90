! @@name:	tasking.1f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success

       RECURSIVE SUBROUTINE traverse ( P )
          TYPE Node
             TYPE(Node), POINTER :: left, right
          END TYPE Node
          TYPE(Node) :: P

          IF (associated(P%left)) THEN
             !$OMP TASK     ! P is firstprivate by default
                 CALL traverse(P%left)
             !$OMP END TASK
          ENDIF
          IF (associated(P%right)) THEN
             !$OMP TASK     ! P is firstprivate by default
                 CALL traverse(P%right)
             !$OMP END TASK
          ENDIF
          CALL process ( P )

       END SUBROUTINE
