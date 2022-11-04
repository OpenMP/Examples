/*
* @@name:	cas.2
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define N 10

typedef struct Node{ struct Node *next; int id; } Node;
typedef struct Queue{ Node *head; Node *tail;   } Queue;

void enqueue( Queue *,  Node * );

int main(){
  Queue  q;
  Node nodes[N];
  int  id_check[N];

// Initializing
  for(int i=0; i<N; i++){ 
     nodes[i].next=NULL; nodes[i].id=i; id_check[i]=-1;
  }

  q.tail=&nodes[0];   // Fill initial tail

// Enqueue
  #pragma omp parallel for num_threads(2)
  for(int i=1; i<N; i++){ 
     enqueue(&q,&nodes[i]);
  }

// Checking Results Below
  Node *node=q.tail;
  do{
     id_check[node->id]=node->id;  //Store found id at position id 
     node =node->next;
  }while(node->next != NULL);
  id_check[node->id]=node->id;    //checking also the 1st node here

  for(int id=0; id<N; id++){     // all ids should be found
     if(id != id_check[id]) {printf("FAILED\n"); exit(1);} 
  }
  printf("PASSED\n");

  return 0;
}

void enqueue(Queue *queue,  Node *node) {
  bool result = false;

  #pragma omp atomic read     
  node->next = queue->tail;
  do{
    #pragma omp atomic compare capture
    {
      result = queue->tail == node->next;
      if(result) {
        queue->tail = node; 
      }else{
        node->next = queue->tail;
      }
    }
  }while(!result);
}
