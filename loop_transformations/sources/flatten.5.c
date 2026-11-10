/*
* @@name:       flatten.5
* @@type:       C++
* @@operation:  run
* @@expect:     success
* @@version:    omp_6.1
*/
int main(){
  std::vector<const char*> list{"A", "B", "C"};

  #pragma omp flatten depth(2)
  for(int i = 0; i<3; i++) {
    for (const char *item : list) {
        printf("i=%i, item=%s\n", i, item);
    }
  }

  return 0;
}



int main_equivalent_step1() {
  std::vector<const char*> list{"A", "B", "C"};

  #pragma omp flatten depth(2) // pseudocode, does not work
  for(int i = 0; i < 3; i++) {

    {
      auto && __range = list;
      for ( auto __begin = __range.begin(),
                          __end = __range.end();
            __begin != __end;
            ++__begin ) {
        const char *item = *__begin;
        printf("i=%i, item=%s\n", i, item);
      }
    }

  }

  return 0;
}



int main_equivalent_step2() {
  std::vector<const char*> list{"A", "B", "C"};

  #pragma omp flatten depth(2) // pseudocode, does not work
  for(int i = 0; i < 3; i++) {

    {
      auto &&__range = list;
      auto __begin = __range.begin();
      ptrdiff_t __count = __range.end() - __begin;
      for (ptrdiff_t __j = 0; __j < __count; ++__j) {
        const char *item = *(__begin + __j);
        printf("i=%i, item=%s\n", i, item);
      }
    }

  }

  return 0;
}


int main_equivalent_step3() {
  std::vector<const char*> list{"A", "B", "C"};

  auto &&__range = list;
  auto __begin = __range.begin();
  ptrdiff_t __count = __range.end() - __begin;

  #pragma omp flatten depth(2)
  for(int i = 0; i < 3; i++) {
      for (ptrdiff_t __j = 0; __j < __count; ++__j) {
        const char *item = *(__begin + __j);
        printf("i=%i, item=%s\n", i, item);
      }
    }
  }

  return 0;
}


int main_equivalent_step4() {
  std::vector<const char*> list{"A", "B", "C"};

  auto &&__range = list;
  auto __begin = __range.begin();
  ptrdiff_t __count = __range.end() - __begin;

  for(ptrdiff_t __ij = 0; i < 3*__count; ++__ij) {
    int i = __ij / __count;
    ptrdiff_t __j = __ij % __count;
    const char *item = *(__begin + __j);
    printf("i=%i, item=%s\n", i, item);
  }

  return 0;
}



/* Expected output (all steps):

i=0, item=A
i=0, item=B
i=0, item=C
i=1, item=A
i=1, item=B
i=1, item=C
i=2, item=A
i=2, item=B
i=2, item=C

*/
