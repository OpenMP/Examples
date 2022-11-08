/*
* @@name:	allocators.6
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.2
*/
#include <omp.h>
#include <stdio.h>

#pragma omp requires dynamic_allocators

int calc(int i, int j) { return i*j;}
#pragma omp declare target(calc)

int main()
{
    #define N 256
    int sum;
    int xbuf[N];

    static omp_allocator_handle_t cgroup_alloc;
    #pragma omp declare target(cgroup_alloc)
    const omp_alloctrait_t cgroup_traits[1] = 
                           {{omp_atk_access, omp_atv_cgroup}};

/*** CASE 1: ***/

    for (int i = 0; i < N; i++) { xbuf[i] = 0;}

    // uses predefined allocator, no need to declare it in uses_allocators
    #pragma omp target teams reduction(+:xbuf) thread_limit(N) \
                      allocate(omp_cgroup_mem_alloc:xbuf) num_teams(4)
    {
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            xbuf[i] += calc(i,omp_get_team_num());
        }
    }

    sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += xbuf[i];
    }
    if(sum == 3*(N-1)*N) printf("PASSED 1 of 2\n");


/*** CASE 2: ***/

    for (int i = 0; i < N; i++) { xbuf[i] = 0; }

    // initializes the allocator in target region
    #pragma omp target
        cgroup_alloc = omp_init_allocator( 
                       omp_default_mem_space, 1, cgroup_traits);

    // uses the initialized allocator
    #pragma omp target
    #pragma omp teams reduction(+:xbuf) thread_limit(N) \
                      allocate(cgroup_alloc:xbuf) num_teams(4)
    {
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            xbuf[i] += calc(i,omp_get_team_num());
        }
    }

    // destroys the allocator after its use
    #pragma omp target
        omp_destroy_allocator(cgroup_alloc);

    sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += xbuf[i];
    }
    if(sum == 3*(N-1)*N) printf("PASSED 2 of 2\n");

    return 0;
}
