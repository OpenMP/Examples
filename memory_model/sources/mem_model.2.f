! @@name:	mem_model.2
! @@type:	F-fixed
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	rt-error
! @@version:	omp_3.1
       PROGRAM EXAMPLE
       INCLUDE "omp_lib.h" ! or USE OMP_LIB
       INTEGER DATA
       INTEGER FLAG, FLAG_VAL

       FLAG = 0
!$OMP  PARALLEL NUM_THREADS(2)
         IF(OMP_GET_THREAD_NUM() .EQ. 0) THEN
         ! Write to the data buffer that will be read by thread 1
            DATA = 42

         ! Flush DATA to thread 1 and strictly order the write to DATA
         ! relative to the write to the FLAG
!$OMP       FLUSH(FLAG, DATA)

         ! Set FLAG to release thread 1
!$OMP       ATOMIC WRITE
            FLAG = 1

         ELSE IF(OMP_GET_THREAD_NUM() .EQ. 1) THEN
         ! Loop until we see the update to the FLAG
!$OMP       FLUSH(FLAG, DATA)
            FLAG_VAL = 0
            DO WHILE(FLAG_VAL .LT. 1)
!$OMP          ATOMIC READ
               FLAG_VAL = FLAG
            ENDDO

         ! Value of FLAG is 1; value of DATA is undefined
            PRINT *, 'FLAG=', FLAG, ' DATA=', DATA

!$OMP       FLUSH(FLAG, DATA)
         ! Value of FLAG is 1; value of DATA is 42
            PRINT *, 'FLAG=', FLAG, ' DATA=', DATA

         ENDIF
!$OMP  END PARALLEL
       END
