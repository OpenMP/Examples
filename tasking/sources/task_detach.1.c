/*
* @@name:	task_detach.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.0
*/
#include <omp.h>

void async_work(void (*)(void*), void*);
void work();

int main() {
  int async=1;
  #pragma omp parallel
  #pragma omp masked
  {

    omp_event_handle_t event;
    #pragma omp task detach(event)
    { 
      if(async) {
        async_work( (void (*)(void*)) omp_fulfill_event, (void*) event );
      } else {
        work();
        omp_fulfill_event(event); 
      }
    }
                  // Other work
    #pragma omp taskwait
  }
  return 0;
}
