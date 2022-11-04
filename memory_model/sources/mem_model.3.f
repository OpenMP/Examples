! @@name:	mem_model.3
! @@type:	F-fixed
! @@operation:	run
! @@expect:	unspecified
! @@version:	omp_3.1
       PROGRAM EXAMPLE
       INCLUDE "omp_lib.h" ! or USE OMP_LIB
       INTEGER FLAG, FLAG_VAL
       INTEGER DATA0, DATA1

       FLAG = 0
!$OMP  PARALLEL NUM_THREADS(3)
         IF(OMP_GET_THREAD_NUM() .EQ. 0) THEN
             DATA0 = 17
!$OMP        FLUSH

         ! Set flag to release thread 1
!$OMP        ATOMIC UPDATE
             FLAG = FLAG + 1
         ! Flush of FLAG is implied by the atomic directive

         ELSE IF(OMP_GET_THREAD_NUM() .EQ. 1) THEN
         ! Loop until we see that FLAG reaches 1
             FLAG_VAL = 0
             DO WHILE(FLAG_VAL .LT. 1)
!$OMP           ATOMIC READ
                FLAG_VAL = FLAG
             ENDDO
!$OMP        FLUSH

         ! DATA0 is 17 here
             PRINT *, 'Thread 1 awoken. DATA0 = ', DATA0

             DATA1 = 42
!$OMP        FLUSH(DATA1)

         ! Set FLAG to release thread 2
!$OMP        ATOMIC UPDATE
             FLAG = FLAG + 1
         ! Flush of FLAG is implied by the atomic directive

         ELSE IF(OMP_GET_THREAD_NUM() .EQ. 2) THEN
         ! Loop until we see that FLAG reaches 2
             FLAG_VAL = 0
             DO WHILE(FLAG_VAL .LT. 2)
!$OMP           ATOMIC READ
                FLAG_VAL = FLAG
             ENDDO
!$OMP        FLUSH(DATA0, DATA1)

         ! There is a data race here; data0 is 17 and data1 is undefined
             PRINT *, 'Thread 2 awoken. DATA0 = ', DATA0,
     &                ' and DATA1 = ', DATA1

         ENDIF
!$OMP  END PARALLEL
       END
