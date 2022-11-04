/*
* @@name:       pause_resource.1
* @@type:       C
* @@operation:  link
* @@expect:     success
* @@version:    omp_5.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <omp.h>

int main()
{
   pid_t pid;
   int nt = omp_get_max_threads();

   #pragma omp parallel
   {
      #pragma omp single
         printf("number of threads = %d (max = %d)\n",
                 omp_get_num_threads(), nt);
   }

   /* clean up thread environment before fork */
   omp_pause_resource(omp_pause_hard, omp_get_initial_device());

   pid = fork();
   if (pid < 0) {
      printf("fork failed\n");
      exit(1);
   }
   else if (pid == 0) {
      /* child process */
      #pragma omp parallel num_threads(nt)
      {
         int myid = omp_get_thread_num();
         printf("child: myid %d of %d\n", myid, nt);
      }
      exit(0);
   }
   else {
      /* parent process */
      int s;
      printf("parent process - waiting pid %d\n", pid);
      waitpid(pid, &s, 0);
   }
   return 0;
}
