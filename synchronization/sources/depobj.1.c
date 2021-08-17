/*
* @@name:       depobj.1c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.0
*/

#include <stdio.h>
#include <omp.h>

#define N 100
#define TRUE  1
#define FALSE 0

void driver(int update, float a[], float b[], int n, omp_depend_t *obj);

void update_copy(int update, float a[], float b[], int n);
void checkpoint(float a[],int n);
void init(float a[], int n);


int main(){

   float a[N],b[N];
   omp_depend_t obj;

   init(a, N);

   #pragma omp depobj(obj) depend(inout: a)

   driver(TRUE,  a,b,N, &obj);  // updating a occurs

   #pragma omp depobj(obj) update(in)

   driver(FALSE, a,b,N, &obj);  // no updating of a

   #pragma omp depobj(obj) destroy  // obj is set to uninitilized state,
                                    // resources are freed
   return 0;

}

void driver(int update, float a[], float b[], int n, omp_depend_t *obj)
{
   #pragma omp parallel num_threads(2)
   #pragma omp single
   {

     #pragma omp task depend(depobj: *obj) // Task 1, uses depend object
       update_copy(update, a,b,n);       // update a or not, always copy a to b
 
     #pragma omp task depend(in: a[:n])    // Task 2, only read a 
       checkpoint(a,n);

   }
}

void update_copy(int update, float a[], float b[], int n)
{ 
   if(update) for(int i=0;i<n;i++) a[i]+=1.0f; 

   for(int i=0;i<n;i++) b[i]=a[i]; 
}

void checkpoint(float a[], int n)
{  
   for(int i=0;i<n;i++) printf(" %f ",a[i]);
   printf("\n");
}

void init(float a[], int n)
{  
   for(int i=0;i<n;i++) a[i]=i; 
}

