/*
* @@name:       task_detach.2c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.0
*/

// use -lrt on loader line
#include  <stdio.h>
#include <unistd.h>
#include  <fcntl.h>
#include    <aio.h>
#include  <errno.h>
#include <signal.h>

#include    <omp.h>

#define IO_SIGNAL SIGUSR1           // Signal used to notify I/O completion

                                    // Handler for I/O completion signal
static void callback_aioSigHandler(int sig, siginfo_t *si, void *ucontext) {
   if (si->si_code == SI_ASYNCIO){
      printf( "OUT: I/O completion signal received.\n");
      omp_fulfill_event( (omp_event_handle_t)(si->si_value.sival_ptr) );
   }
}

void work(int i){ printf("OUT: Executing work(%d)\n", i);}

int main() {
    // Write "Written Asynchronously." to file data, using POSIX asynchronous IO
    // Error checking not included for clarity and simplicity.

   char      data[] = "Written Asynchronously.";

   struct     aiocb cb;
   struct sigaction sa;

   omp_event_handle_t event;

   int fd = open(  "async_data", O_CREAT|O_RDWR|O_TRUNC,0664);

   // Setup async io (aio) control block (cb)
   cb.aio_nbytes  = sizeof(data)-1;
   cb.aio_fildes  = fd;
   cb.aio_buf     = data;
   cb.aio_reqprio = 0;
   cb.aio_offset  = 0;
   cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
   cb.aio_sigevent.sigev_signo  = IO_SIGNAL;

   // Setup Signal Handler  Callback
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART | SA_SIGINFO;
   sa.sa_sigaction = callback_aioSigHandler;   //callback
   sigaction(IO_SIGNAL, &sa, NULL);

  #pragma omp parallel num_threads(2)
  #pragma omp masked 
  {
  
    #pragma omp task detach(event) if(0)               // TASK1
    {
       cb.aio_sigevent.sigev_value.sival_ptr = (void *) event;
       aio_write(&cb);
    }
       
    #pragma omp task                                   // TASK2
       work(1);
    #pragma omp task                                   // TASK3
       work(2);

  } // Parallel region barrier ensures completion of detachable task.

  // Making sure the aio operation completed.
  // With OpenMP detachable task the condition will always be false:
  while(aio_error(&cb) == EINPROGRESS){printf(" INPROGRESS\n");} //Safeguard

   close(fd);
   return 0;
}
/* Any Order:
OUT: I/O completion signal received.
OUT: Executing work(1)
OUT: Executing work(2)
*/
