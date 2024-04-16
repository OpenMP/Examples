/*
* @@name:	target_update.3
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct{
   int x;
   int y;
   int z;
}T;

#pragma omp declare mapper(custom: T S) map(to:S.x) \
                    map(from:S.y) map(alloc: S.z)

int main()
{
    T s;

    s.x = 5;
    s.y = 5;
    s.z = 5;
    #pragma omp target data map(mapper(custom),tofrom: s)
    {
        int a,b,c;
        s.x += 5;
        s.y += 5;
        s.z += 5;

        #pragma omp target update to(mapper(custom): s) 
        // becomes #pragma omp target update to(s.x)

        #pragma omp target map(from: a,b,c)
        {
            a = s.x;
            b = s.y;            //s.y is undefined here
            c = s.z;            //s.z is undefined here

            s.y = 5;

            printf("s.x:%d, s.y:%d \n", s.x, s.y);
                 // s.x:10, s.y:5 (value of s.z is undefined)
        }
        #pragma omp target update from(mapper(custom): s)
        // becomes #pragma omp target update from(s.y)

        printf("s.y:%d \n", s.y);
             // s.y:5
        printf("a:%d \n", a);
             // a:10 (values of b and c are undefined)
    }
    printf("s.x:%d, s.y:%d, s.z:%d\n", s.x, s.y, s.z);
         // s.x:10, s.y:5, s.z:10

    return 0;
}
