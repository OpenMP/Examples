/*
* @@name:       target_defaultmap.1.c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:	omp_5.0
*/
#include <stdlib.h>
#include <stdio.h>
#define N 2

int main(){
  typedef struct S_struct { int s; int A[N]; } S_struct_t;
  
  
  int         s;       //scalar int variable (scalar)
  int         A[N];    //aggregate variable  (array)
  S_struct_t  S;       //aggregate variable  (structure)
  int         *ptr;    //scalar, pointer variable (pointer)

  int         s1, s2, s3;

// Initialize everything to zero;
   s=2; s1=s2=s3=0; 
   A[0]=0; A[1]=0;
   S.s=0; S.A[0]=0; S.A[1]=0;
    
// Target Region 1
                   // Uses defaultmap to set scalars, aggregates & pointers 
                   // to normal defaults.  
    #pragma omp target \
            defaultmap(firstprivate: scalar)     //could also be default \
            defaultmap(tofrom:       aggregate)  //could also be default \
            defaultmap(default:      pointer)    //must be default       \
            map(ptr2m[:N])
    {
        s       = 3;               // SCALAR firstprivate, value not returned
 
        A[0]    = 3;  A[1] = 3;    // AGGREGATE array, default map tofrom
 
                                    // AGGREGATE structure, default tofrom
        S.s     = 2;
        S.A[0]  = 2;  S.A[1] = 2;

        ptr = &A[0];                // POINTER is private
        ptr[0] = 2;   ptr[1] = 2;

    }
   if(s==2 && A[0]==2 && S.s==2 && S.A[0]==2) 
      printf(" PASSED 1 of 4\n");
  
    
// Target Region 2
                   // no implicit mapping allowed.
    #pragma omp target defaultmap(none) map(tofrom: s, A, S)
    {                   
        s     +=5;           // All variables must be explicitly mapped
        A[0]  +=5; A[1]+=5;
        S.s   +=5;
        S.A[0]+=5; S.A[1]+=5; 
    }
    if(s==7 && A[0]==7 && S.s==7 && S.A[0]==7) printf(" PASSED 2 of 4\n");
   
  
// Target Region 3
                  // defaultmap & explicit map with variables in same category
    s1=s2=s3=1;
    #pragma  omp defaultmap(tofrom: scalar) map(firstprivate: s1,s2) 
    {
        s1 += 5;           // firstprivate (s1 value not returned to host)
        s2 += 5;           // firstprivate (s2 value not returned to host)
        s3 += s1 + s2;     // mapped as tofrom
    }
    if(s1==1 && s2==1 && s3==13 ) printf(" PASSED 3 of 4\n");
 
 
// Target Region 4        
    A[0]=0; A[1]=0;
    S.A[0]=0; S.A[1]=0;

    // arrays and structure are firstprivate, and scalars are from
    #pragma omp target defaultmap(firstprivate: aggregate) map(from: s1, s2)
    {

        A[0]+=1; S.A[0]+=1; //Aggregate changes not returned to host
        A[1]+=1; S.A[1]+=1; //Aggregate changes not returned to host
        s1 = A[0]+S.A[0]; //s1 value returned to host
        s2 = A[1]+S.A[1]; //s1 value returned to host
    }
    if( A[0]==0 && S.A[0]==0 && s1==2 ) printf(" PASSED 4 of 4\n");
 
}
