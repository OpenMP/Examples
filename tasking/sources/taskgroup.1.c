/*
* @@name:	taskgroup.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/
extern void start_background_work(void);
extern void check_step(void);
extern void print_results(void);
struct tree_node
{
   struct tree_node *left;
   struct tree_node *right;
};
typedef struct tree_node* tree_type;
extern void init_tree(tree_type);
#define max_steps 100
void compute_something(tree_type tree)
{
   // some computation
}
void compute_tree(tree_type tree)
{
   if (tree->left)
   {
     #pragma omp task
       compute_tree(tree->left);
   }
   if (tree->right)
   {
     #pragma omp task
       compute_tree(tree->right);
   }
   #pragma omp task
   compute_something(tree);
}
int main()
{
  int i;
  tree_type tree;
  init_tree(tree);
  #pragma omp parallel
  #pragma omp single
  {
    #pragma omp task
      start_background_work();
    for (i = 0; i < max_steps; i++)
    {
        #pragma omp taskgroup
        {
           #pragma omp task
             compute_tree(tree);
        } // wait on tree traversal in this step
        check_step();
    }
  } // only now is background work required to be complete
  print_results();
  return 0;
}
