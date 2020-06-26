/*
* @@name:	tasking.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.0
*/

struct node {
  struct node *left;
  struct node *right;
};

extern void process(struct node *);

void traverse( struct node *p ) 
{
  if (p->left)
#pragma omp task   // p is firstprivate by default
      traverse(p->left);
  if (p->right)
#pragma omp task    // p is firstprivate by default
      traverse(p->right);
  process(p);
}
